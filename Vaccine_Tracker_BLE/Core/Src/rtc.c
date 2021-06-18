
#include "rtc.h"

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void) {

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  //hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  //hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
    while (1);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) {
  
  if (rtcHandle->Instance == RTC) {
    __HAL_RCC_RTC_CLK_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) {
  
  if (rtcHandle->Instance == RTC) {
    __HAL_RCC_RTC_CLK_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  }
}
