/**
 *
 * Copyright (c) 2020 HT Micron Semicondutors S.A.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "main.h"
#include "tim.h"
#include "HT_sigfox_api.h"
#include "HT_hcfsm.h"

extern uint8_t asc2_data_flag;

char err_str[10];

void HT_SigfoxApi_configRegion(rc_mask RCZ) {
	ST_SFX_ERR open_err = ST_SFX_ERR_NONE;

	//printf("Config region...\n");

	switch(RCZ){
	case RCZ1:
		//printf("RCZ %d\n", RCZ);
		ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
		open_err = St_Sigfox_Open_RCZ(RCZ1);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	case RCZ2:
		//printf("RCZ %d\n", RCZ);
		ST_RF_API_reduce_output_power(RCZ2_OUTPUT_POWER);
		open_err = St_Sigfox_Open_RCZ(RCZ2);
		HT_SigfoxApi_switchPa(1);
		HT_SigfoxApi_setSmpsVoltageAction(2);

		break;
	case RCZ3:
		//printf("RCZ %d\n", RCZ);
		open_err = St_Sigfox_Open_RCZ(RCZ3);
		ST_RF_API_reduce_output_power(RCZ3_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);


		break;
	case RCZ4:
		//printf("RCZ %d\n", RCZ);
		open_err = St_Sigfox_Open_RCZ(RCZ4);
		ST_RF_API_reduce_output_power(RCZ4_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(1);
		HT_SigfoxApi_setSmpsVoltageAction(2);


		break;
	case RCZ5:
		open_err = St_Sigfox_Open_RCZ(RCZ5);
		ST_RF_API_reduce_output_power(RCZ5_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);


		break;
	case RCZ6:
		//printf("RCZ %d\n", RCZ);
		open_err = St_Sigfox_Open_RCZ(RCZ6);
		ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	case RCZ7:
		//printf("RCZ %d\n", RCZ);
		open_err = St_Sigfox_Open_RCZ(RCZ7);
		ST_RF_API_reduce_output_power(RCZ7_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(1);

		break;
	default:
		break;
	}

	//HAL_Delay(1000);
	sprintf(err_str, "0x%X", open_err);
	//printf(err_str);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, (uint8_t *)err_str, AT_REPONSE_BUFFER_SIZE, 100);
	printUart(err_str);

}

void HT_SigfoxApi_switchPa(uint8_t state) {
	ST_RF_API_set_pa(state);
	//printf("Switch PA: %d\n", state);
}

void HT_SigfoxApi_setSmpsVoltageAction(sfx_u8 mode) {
	ST_RF_API_smps(mode);
	//printf("Set_smps_voltage %d\n", mode);
}

void HT_SigfoxApi_closeSigfoxLib(void) {
	sfx_error_t err;

	err = SIGFOX_API_close();
	sprintf(err_str, "0x%X", err);

	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, (uint8_t *)err_str, AT_REPONSE_BUFFER_SIZE, 100);
	//printf(err_str);
	printUart(err_str);
}

sfx_error_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_u8 *customer_response, sfx_bool initiate_downlink_flag, uint8_t len) {
	uint8_t buffSize;
	sfx_error_t err;

	if(asc2_data_flag)
		buffSize = len;
	else
		buffSize = !(len % 2) ? len/2 : (len/2)+1;

	if(initiate_downlink_flag)
		HAL_TIM_Base_Start_IT(&htim21);

	err = SIGFOX_API_send_frame(customer_data, buffSize,customer_response, 3, initiate_downlink_flag);

	if(initiate_downlink_flag && !err) {
		printf("Customer resp: {");

		for(uint16_t i = 0; i < 7; i++)
			printf("0x%x,", customer_response[i]);

		printf("0x%x}\n\r", customer_response[7]);
	}

	sprintf(err_str, "0x%X", err);
	//printf(err_str);

	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, (uint8_t *)err_str, AT_REPONSE_BUFFER_SIZE, 100);
	printUart(err_str);

	if(initiate_downlink_flag)
		HAL_TIM_Base_Stop_IT(&htim21);

	return err;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
