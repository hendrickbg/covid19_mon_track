/**
******************************************************************************
* @file    bluenrg_lp_it.c 
* @author  VMA RF Application Team
* @version V1.0.0
* @date    29-March-2018
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and
*          peripherals interrupt service routine.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_lp_it.h"
#include "ble_const.h"
#include "bluenrg_lp_stack.h"
#include "bluenrg_lp_hal_vtimer.h"
#include "hal_miscutil.h"
#include "crash_handler.h"

#include "usart.h"
#include "HT_gatt_db.h"

extern EXTI_HandleTypeDef HEXTI_Seal;

extern volatile uint8_t deepSleepModeFlag;
volatile uint32_t debounce_count = 0;
uint32_t debounce_last = 0;

extern uint8_t tim_counter;

uint8_t wkp_flag = 0;
uint8_t init_device = 1;

extern HT_Fsm fsm_state;

/** @addtogroup BlueNRG1_StdPeriph_Examples
* @{
*/

/** @addtogroup GPIO_Examples
* @{
*/

/** @addtogroup GPIO_IOToggle
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  */
NOSTACK_FUNCTION(NORETURN_FUNCTION(void NMI_IRQHandler(void)))
{
  HAL_CrashHandler(__get_MSP(), NMI_SIGNATURE);
  /* Go to infinite loop when NMI exception occurs */
  while (1)
  {
  }
}

void HardFault_IRQHandler(void) {

  printf("HardFault!");
  NVIC_SystemReset();

  while (1);
}

/**
  * @brief  This function handles SVCall exception.
  */
void SVC_IRQHandler(void)
{
}

/**
* @brief  This function handles SysTick Handler.
*/
void SysTick_IRQHandler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  debounce_count++;
}

/******************************************************************************/
/*                 BLUENRG_LP Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (EXTI), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_bluenrg_lp.s).                                               */
/******************************************************************************/
/**
* @brief  This function handles UART interrupt request.
* @param  None
* @retval None
*/

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

/**
* @brief  This function handles DMA Handler.
*/
void DMA_IRQHandler(void) {
  
}

/**
* @brief  This function handles GPIO interrupt request.
* @param  None
* @retval None
*/

void GPIOB_IRQHandler(void) {

  static uint32_t debounce_last = 0;
  
  if(HAL_EXTI_GetPending(&HEXTI_Seal)){
    if ( (debounce_count - debounce_last) >= 35 )
    { 
      /* Add the SW no bounce */
      debounce_last = debounce_count;
      
      /* Handle user button press in dedicated function */
      HAL_EXTI_IRQHandler(&HEXTI_Seal );
    }
    LL_EXTI_ClearInterrupt(HEXTI_Seal.Line);
  }

}

void GPIOA_IRQHandler(void)
{

}

void TIM1_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1);
}

void RTC_IRQHandler(void) {

  if (deepSleepModeFlag) {
  
		HAL_NVIC_DisableIRQ(RTC_IRQn);
		__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    deepSleepModeFlag = 0;
    wkp_flag = 1;

    HT_APP_WakeupHandle();

    tim_counter++;

    fsm_state = HT_SM_TEMPERATURE_HANDLER;

  } else if(init_device) { 
    init_device = 0;
    
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  }
}

/* User callback to be called if an interrupt is associated to a wakeup source */
void HAL_PWR_MNGR_WakeupIOCallback(uint32_t source) {
}

void BLE_WKUP_IRQHandler(void)
{
  HAL_VTIMER_WakeUpCallback();
}

void CPU_WKUP_IRQHandler(void)
{
  HAL_VTIMER_TimeoutCallback();
}

void BLE_ERROR_IRQHandler(void) {
  volatile uint32_t debug_cmd;

  BLUE->DEBUGCMDREG |= 1;

  /* If the device is configured with 
     System clock = 64 MHz and BLE clock = 16 MHz
     a register read is necessary to end fine  
     the clear interrupt register operation,
     due the AHB down converter latency */
  debug_cmd = BLUE->DEBUGCMDREG;
}

void BLE_TX_RX_IRQHandler(void) {
  uint32_t blue_status = BLUE->STATUSREG;
  uint32_t blue_interrupt = BLUE->INTERRUPT1REG;

  /** clear all pending interrupts */
  BLUE->INTERRUPT1REG = blue_interrupt;

  BLE_STACK_RadioHandler(blue_status | blue_interrupt);
  HAL_VTIMER_RadioTimerIsr();

  /* If the device is configured with 
     System clock = 64 MHz and BLE clock = 16 MHz
     a register read is necessary to end fine  
     the clear interrupt register operation,
     due the AHB down converter latency */
  blue_interrupt = BLUE->INTERRUPT1REG;
}
/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
