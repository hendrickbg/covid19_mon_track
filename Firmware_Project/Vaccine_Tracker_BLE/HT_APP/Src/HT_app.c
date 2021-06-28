/*****************************************************************************
* File Name          : HT_app.c
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : HT app source code. 
*******************************************************************************/

#include "HT_app.h"
#include "HT_ble_api.h"

volatile uint8_t set_connectable = 0;

uint8_t usart_callback = 0;
volatile uint8_t usart_flag = 0;

uint8_t cmd_flag = 1;
char err_none[] = "0x0";

char usart_buffer[4];

uint8_t connected = 0;

WakeupSourceConfig_TypeDef wakeupIO; 
PowerSaveLevels stopLevel;

HT_Fsm fsm_state = HT_SM_TEMPERATURE_HANDLER;

double current_temp = 0.0, p_current_temp;
double max_temp = -100.0, p_max_temp, min_temp = 100.0, p_min_temp;

HT_Seal_State seal_state = HT_SEAL_CLOSED;
HT_Event_Type event_type = HT_PERIODIC_EVENT;

uint8_t tim_counter = 0;
volatile uint8_t deepSleepModeFlag = 0;
volatile uint8_t gpio_callback = 0;

volatile uint8_t timeout_flag = 0;
uint8_t timeout_counter = 0;
uint8_t error_counter = 0;
extern uint8_t callback_counter;
uint32_t sleep_cont = 0;
uint8_t seal_irqn = 0;

uint8_t batt_lvl = 0;
extern uint8_t wkp_flag;

uint8_t clear_flag = 0;

EPD epd;

void HT_APP_ConfigWakeupIO(void) {
	wakeupIO.RTC_enable = 1;
	wakeupIO.IO_Mask_Low_polarity = WAKEUP_PB4;
	wakeupIO.IO_Mask_High_polarity = 0;
}

float HT_APP_GetTemperature(void) {
	HT_APP_ReadTemperatureSensor(1);

	return (float)current_temp;
}

void HT_APP_SaveContext(void) {
	p_current_temp = current_temp;
	p_max_temp = max_temp;
	p_min_temp = min_temp;
}

void HT_APP_BleTick(void) {
	if (set_connectable) {
    	HT_BLE_SetDeviceConnectable();
    	set_connectable = FALSE;
	}

	ModulesTick();
}

void HT_APP_ResetInfo(void) {
	event_type = HT_PERIODIC_EVENT;
}

void HT_APP_EnableRTCWkp(uint32_t seconds) {
	
	/* Clear RTC Wake Up timer Flag */
	LL_RTC_ClearFlag_WUT(RTC);
	
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);

	HAL_NVIC_EnableIRQ(RTC_IRQn);

    /* Set wakeup IT */
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
}

void HT_APP_WakeupHandle(void) {
	MX_GPIO_Init();
	MX_ADC_Init();
	MX_DMA_Init();
	MX_TIM1_Init();
	MX_SPI1_Init();
}

void HT_APP_SetNewState(HT_Fsm new_state) {
	fsm_state = new_state;
}

void HT_APP_StopTimer(void) {

	callback_counter = 0;

	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Stop_IT(&htim1);
	HAL_TIM_Base_DeInit(&htim1);
}

void HT_APP_InitTimeout(void) {

	callback_counter = 0;

	HAL_NVIC_DisableIRQ(RTC_IRQn);
	
	MX_TIM1_Init();
	HAL_Delay(50);

	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
	if(HAL_TIM_Base_Start_IT(&htim1) != HAL_OK) {
		printf("HAL_TIM_Base_Start_IT Error!\n");
	}
}

void HT_APP_WakeUpHT32(void) {

	printf("Waking up HT32SX...\n");

	HT_APP_ResetSigfoxMod();

	usart_flag = 0;
}

uint8_t HT_APP_Alert(void) {
	float min_thr, max_thr;

	min_thr = MIN_TEMPERATURE-TOLERANCE;
	max_thr = MAX_TEMPERATURE+TOLERANCE;

	/* Compare values */
	if((current_temp < min_thr || current_temp > max_thr) && (event_type != HT_BUTTON_EVENT && event_type != HT_SEAL_EVENT)) {
		event_type = HT_TEMPERATURE_EVENT;
		return 1;
	} else if(seal_irqn) {
		seal_irqn = 0;
		return 1;
	} else if(event_type == HT_BUTTON_EVENT)
		return 1;

    return 0;
}

void HT_APP_GetInfos(HT_payload *payload) {
    payload->event_type = event_type;
	payload->max_temperature = (int16_t)(max_temp*100);
	payload->min_temperature = (int16_t)(min_temp*100);
	payload->current_temperature = (int16_t)(current_temp*100);
	payload->battery_level = batt_lvl;
	payload->seal_state = seal_state;
}

void HT_APP_ResetContext(void) {
	memset(usart_buffer, 0, sizeof(usart_buffer));
	usart_callback = 0;
	timeout_counter = 0;
	cmd_flag = 1;
	error_counter = 0;
	timeout_flag = 0;
}

void HT_APP_WaitForResponse(void) {
	usart_callback = 0;

	timeout_counter = 0;

	while (1) {
		if(cmd_flag) {
			memset(usart_buffer, 0, sizeof(usart_buffer));
		
			HAL_UART_Receive_IT(&huart1, (uint8_t *)usart_buffer, USART_BUFFER_SIZE);
			HT_APP_InitTimeout();

			cmd_flag = 0;
			usart_callback = 0;
		}

		if(usart_callback) {
			
			HT_APP_StopTimer();
			if(!strncmp(usart_buffer, "HT3", USART_BUFFER_SIZE)) 
				break;
			else 
				cmd_flag = 1;
		} 

		if(timeout_flag) {
			HT_APP_StopTimer();

			timeout_flag = 0;
			fsm_state = HT_SM_RESET_SIGFOX_MOD;
			break;
		}

		HT_APP_BleTick();
	}

	HT_APP_StopTimer();
	HT_APP_ResetContext();
}

void HT_APP_SendATCommand(char *cmd) {
	uint8_t clear_buff;
	usart_callback = 0;

	while (1) {
		
		if(cmd_flag) {
			memset(usart_buffer, 0, sizeof(usart_buffer));
			HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 0xFF);

			if(clear_flag){
				HAL_UART_Receive(&huart1, &clear_buff, 1, 100);
				clear_flag = 0;
			}

			HAL_UART_Receive_IT(&huart1, (uint8_t *)usart_buffer, USART_BUFFER_SIZE);
			HT_APP_InitTimeout();

			cmd_flag = 0;
			usart_callback = 0;
		
		} else if(usart_callback) {
			HT_APP_StopTimer();

			if(!strncmp(usart_buffer, err_none, USART_BUFFER_SIZE)) {
				break;
			
			} else if(error_counter < 3) {
				error_counter++;
				cmd_flag = 1;
				HAL_Delay(500);
			} else {
				break;
			}
			
		} else if(gpio_callback) {
			HT_APP_StopTimer();
			gpio_callback = 0;
			break;
		
		} else if(timeout_flag) {
			HT_APP_StopTimer();

			if(timeout_counter < REPETITION_COUNTER) {
				timeout_counter++;
				cmd_flag = 1;
				timeout_flag = 0;
			
				HAL_Delay(500);
			} else {
				fsm_state = HT_SM_RESET_SIGFOX_MOD;
				break;
			}
		}

		HT_APP_BleTick();
	}

	HT_APP_StopTimer();
	HT_APP_ResetContext();
}

void HT_APP_ResetSigfoxMod(void) {
	
	/* Reset HT32SX */
	GPIOB->BSRR = 1 << 17;
	HAL_Delay(50);
	GPIOB->BSRR = 1 << 1;
	HAL_Delay(50);
}

void HT_APP_ReadTemperatureSensor(uint8_t ble_resp) {
	float v_out;
	float this_temp = 0.0;
	float battery_lvl;

	/* Turn on NTC sensor. */
	GPIOA->BSRR = 1 << 5;

	/* Read temperature */
	HT_ADC_ReadFloatAdc(&v_out, &battery_lvl);

	this_temp = HT_SENSOR_CalculateTemperature(v_out);

	printf("Calculated temperature: %.2f\n", this_temp);
	printf("Battery lvl: %.2f\n", battery_lvl);

	batt_lvl = (uint8_t)(battery_lvl*10);

	current_temp = this_temp;

	max_temp = (max_temp < this_temp) ? this_temp : max_temp;
	min_temp = (min_temp > this_temp) ? this_temp : min_temp;
}

void  HT_APP_InitHT32SX(void) {
	HT_APP_ResetSigfoxMod();
}

void HT_APP_CfgSigfoxRegion(void) {
	char tmp[15];

	sprintf(tmp, "AT+CFGRCZ=%d;", HT_SIGFOX_RCZ);
	HT_APP_SendATCommand(tmp);
}

void HT_APP_SendFrameAction(HT_payload *payload) {
	char tmp[40];
	
	sprintf(tmp, "AT+SEND=0:%02X%04X%04X%04X%02X%02X;", payload->event_type, payload->max_temperature, payload->min_temperature,
		payload->current_temperature, payload->battery_level, payload->seal_state);

	HT_APP_SendATCommand(tmp);
}

void HT_APP_DeepSleepState(void) {

	deepSleepModeFlag = 1;

	if(!connected) {
		HT_ADC_DeInit();
		HAL_SPI_MspInit(&hspi1);
		HAL_TIM_Base_MspInit(&htim1);
		MX_GPIO_LP_Init();
	}

	/* Turn off NTC sensor. */
	GPIOA->BSRR = 1 << 21;
	
	/* Power Save Request */
    HAL_PWR_MNGR_Request(POWER_SAVE_LEVEL_STOP_WITH_TIMER, wakeupIO, &stopLevel);
}

void HT_APP_UpdateDisplay(void) {
	unsigned char frame_buffer[(EPD_WIDTH*EPD_HEIGHT)/8];
	Paint paint;

	char template_str[20];
	char min_temp_str[40];
	char max_temp_str[40];
	char current_temp_str[40];
	char seal_str[30];
	char event_str[30];

	printf("Updating display...\n");

	sprintf(template_str, "Vaccine Status:");
	sprintf(current_temp_str, "Temperature: %.2f C", (float)current_temp);
	sprintf(min_temp_str, "Min: %.2f C", (float)min_temp);
	sprintf(max_temp_str, "Max: %.2f C", (float)max_temp);
	sprintf(seal_str, "Seal Status: %s", (seal_state == HT_SEAL_OPEN) ? "OPEN" : "CLOSED");
	
	if(event_type == HT_SEAL_EVENT)
		sprintf(event_str, "Event: SEAL");
	else if(event_type == HT_BUTTON_EVENT)
		sprintf(event_str, "Event: BUTTON");
	else if(event_type == HT_PERIODIC_EVENT)
		sprintf(event_str, "Event: PERIODIC");
	else 
		sprintf(event_str, "Event: TEMPERATURE");

	if (EPD_Init(&epd, lut_full_update) != 0) 
		printf("e-Paper init failed\n");

	Paint_Init(&paint, frame_buffer, epd.width, epd.height);
	Paint_Clear(&paint, UNCOLORED);

	Paint_DrawStringAt(&paint, 40, 0, template_str, &Font20, COLORED);
	Paint_DrawStringAt(&paint, 0, 34, current_temp_str, &Font12, COLORED);
	Paint_DrawStringAt(&paint, 0, 54, min_temp_str, &Font12, COLORED);
	Paint_DrawStringAt(&paint, 0, 64, max_temp_str, &Font12, COLORED);
	Paint_DrawStringAt(&paint, 0, 114, seal_str, &Font12, COLORED);
	Paint_DrawStringAt(&paint, 150, 114, event_str, &Font12, COLORED);

	/* Display the frame_buffer */
	EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	EPD_DisplayFrame(&epd);
	EPD_DelayMs(&epd, 2000);
	EPD_ClearFrameMemory(&epd, 0xFF);
	EPD_DisplayFrame(&epd);
	EPD_ClearFrameMemory(&epd, 0xFF);
	EPD_DisplayFrame(&epd);

	/* EPD_or partial update */
	if (EPD_Init(&epd, lut_partial_update) != 0) 
		printf("e-Paper init failed\n");
}

void HT_APP_TemparatureHandlerState(void) {

	/* read sensor */
	HT_APP_ReadTemperatureSensor(0);
}

void HT_APP_SendFrameState(HT_payload *payload) {
	
	HT_APP_WakeUpHT32();
	HAL_Delay(1000);
	
	clear_flag = sleep_cont > 0 ? 1 : 0;

	if(fsm_state != HT_SM_RESET_SIGFOX_MOD) {
		HT_APP_CfgSigfoxRegion();
		HAL_Delay(500);
	}
	
	if(fsm_state != HT_SM_RESET_SIGFOX_MOD) {
		HT_APP_SendFrameAction(payload);
		HAL_Delay(500);
	}

	if(fsm_state != HT_SM_RESET_SIGFOX_MOD) {
		HT_APP_SendATCommand("AT+DEEPSLEEP;");
	}	
}

void HT_APP_ButtonHandlerState(void) {
	
	printf("Button handler state\n");

	if(deepSleepModeFlag) {
		__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
		deepSleepModeFlag = 0;
	}

	GPIOA->BSRR = 1 << 5; /* </ Turn on LED */
	HAL_Delay(100);
	GPIOA->BSRR = 1 << 21; /* </ Turn off LED */

	tim_counter = 0;
	event_type = HT_BUTTON_EVENT;
}

void HT_APP_SealHandlerState(void) {
	NVIC_DisableIRQ(GPIOB_IRQn);

	if(deepSleepModeFlag) {
		__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
		deepSleepModeFlag = 0;
	}

	GPIOA->BSRR = 1 << 5; /* </ Turn on LED */
	HAL_Delay(100);
	GPIOA->BSRR = 1 << 21; /* </ Turn off LED */

	seal_state = HT_SEAL_OPEN;
	tim_counter = 0;
	seal_irqn = 1;
	event_type = HT_SEAL_EVENT;

	/* Routine to avoid debouncing */
	__HAL_GPIO_EXTI_CLEAR_IT(GPIOB, SEAL_Pin);
	NVIC_SetPriority(GPIOB_IRQn, 0);
    NVIC_EnableIRQ(GPIOB_IRQn);
}

void HT_APP_CalibrationProcessState(void) {
	float v_out, batt_lvl;

	HT_ADC_ReadFloatAdc(&v_out, &batt_lvl);

	printf("V_out: %.4f\n", v_out);
}

void HT_APP_fsm(void) {
    HT_payload payload = {0};

    switch (fsm_state) {
    case HT_SM_DEEP_SLEEP:
		
       	if(!deepSleepModeFlag) {
			
			wkp_flag = 0;
			sleep_cont++;

			printf("Sleeping: %d\n", sleep_cont);

			HT_APP_EnableRTCWkp(RTC_WAKEUP_TIMEOUT);
			HT_APP_SaveContext();
		}

		if(!wkp_flag)
			HT_APP_DeepSleepState();

		break;

	case HT_SM_TEMPERATURE_HANDLER:

		printf("Temperature handler state...\n");
		HT_APP_TemparatureHandlerState();

		fsm_state = HT_APP_Alert() ? HT_SM_SEND_FRAME : HT_SM_CHECK_TIMER;

		if(max_temp != p_max_temp || min_temp != p_min_temp || current_temp != p_current_temp) 
			HT_APP_UpdateDisplay();

		break;

	case HT_SM_BUTTON_HANDLER:
		
		printf("BLE button handler state...\n");
		HT_APP_ButtonHandlerState();

		fsm_state = HT_SM_TEMPERATURE_HANDLER;

		break;

	case HT_SM_SEAL_HANDLER:

		printf("Seal handler state\n");
		HT_APP_SealHandlerState();

		fsm_state = HT_SM_TEMPERATURE_HANDLER;

		break;

	case HT_SM_SEND_FRAME:

		HT_APP_GetInfos(&payload);
		
		if(event_type != HT_PERIODIC_EVENT && connected)
			HT_BLE_SendNotifyAlert(&payload);
		
		HT_APP_SendFrameState(&payload);

		deepSleepModeFlag = 0;

		if(fsm_state != HT_SM_RESET_SIGFOX_MOD) {
			fsm_state = HT_SM_DEEP_SLEEP;
			HT_APP_ResetInfo();
		} else {
			fsm_state = HT_SM_RESET_SIGFOX_MOD;
		}
		
		break;

	case HT_SM_CHECK_TIMER:

		if(tim_counter == (SEND_FRAME_TIME-1)) {
			tim_counter = 0;
			event_type = HT_PERIODIC_EVENT;
			fsm_state = HT_SM_SEND_FRAME;
		} else {
			deepSleepModeFlag = 0;
			fsm_state = HT_SM_DEEP_SLEEP;
		}

		break;

	case HT_SM_CALIBRATION_PROCESS:

		HT_APP_CalibrationProcessState();
		HAL_Delay(100);
		
		break;

	case HT_SM_RESET_SIGFOX_MOD:
		printf("Reseting HT32SX...\n");

		HT_APP_ResetSigfoxMod();
		fsm_state = HT_SM_SEND_FRAME;

		break;
	default:
		printf("Error! Invalid State!\n");
		break;
	}
    
}

void HT_app(void) {
    
	if (set_connectable) {
        HT_BLE_SetDeviceConnectable();
        set_connectable = FALSE;
    }

    HT_APP_fsm();
}

/************************ HT_app.c *****END OF FILE*****************************/
