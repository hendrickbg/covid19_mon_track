/*****************************************************************************
* File Name          : main.c
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 10-April-2021
* Description        : Vaccine Tracker main file.
*******************************************************************************/

#include "main.h"
#include "HT_ble_api.h"

NO_INIT(uint32_t dyn_alloc_a[DYNAMIC_MEMORY_SIZE >> 2]);

extern WakeupSourceConfig_TypeDef wakeupIO;
extern PowerSaveLevels stopLevel;

int main(void) {

  /* System initialization function */
  if (SystemInit(SYSCLK_64M, BLE_SYSCLK_32M) != SUCCESS)
    while (1);

  HAL_Init();

  /* Configure IOs for power save modes */
  MX_GPIO_LP_Init();

  ModulesInit();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_ADC_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_RTC_Init();

  printf("Vaccine Tracker Application.\n");
  
  HT_BLE_BleConfig();

  HT_BLE_SetDeviceConnectable();
  HT_APP_ConfigWakeupIO();

  while (1) {

    ModulesTick();
    HT_app();
    //HT_APP_DeepSleepState();
  }
}

void ModulesInit(void) {
  uint8_t ret;
  BLE_STACK_InitTypeDef BLE_STACK_InitParams = BLE_STACK_INIT_PARAMETERS;

  LL_AHB_EnableClock(LL_AHB_PERIPH_PKA | LL_AHB_PERIPH_RNG);

  /* BlueNRG-LP stack init */
  ret = BLE_STACK_Init(&BLE_STACK_InitParams);
  if (ret != BLE_STATUS_SUCCESS)
    while (1);

  HAL_VTIMER_InitType VTIMER_InitStruct = {HS_STARTUP_TIME, INITIAL_CALIBRATION, CALIBRATION_INTERVAL};
  HAL_VTIMER_Init(&VTIMER_InitStruct);

  BLEPLAT_Init();
}

void ModulesTick(void) {
  /* Timer tick */
  HAL_VTIMER_Tick();

  /* Bluetooth stack tick */
  BLE_STACK_Tick();

  /* NVM manager tick */
  NVMDB_Tick();
}

PowerSaveLevels App_PowerSaveLevel_Check(PowerSaveLevels level) {
  return POWER_SAVE_LEVEL_STOP_WITH_TIMER;
}

void hci_hardware_error_event(uint8_t Hardware_Code) {
  if (Hardware_Code <= 0x03)
    NVIC_SystemReset();
}

void aci_hal_fw_error_event(uint8_t FW_Error_Type, uint8_t Data_Length, uint8_t Data[]) {
  if (FW_Error_Type <= 0x03) {
    uint16_t connHandle;

    /* Data field is the connection handle where error has occurred */
    connHandle = LE_TO_HOST_16(Data);

    aci_gap_terminate(connHandle, BLE_ERROR_TERMINATED_REMOTE_USER);
  }
}
