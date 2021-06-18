
#include "tim.h"

TIM_HandleTypeDef htim1;

extern volatile uint8_t timeout_flag;
uint8_t callback_counter = 0;

void MX_TIM1_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = PRESCALER_VALUE;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = PERIOD_VALUE;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
        while(1);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
        while(1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();
        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_IRQn, 0);
        HAL_NVIC_EnableIRQ(TIM1_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();

        /* TIM1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM1_IRQn);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM1) {
        if(callback_counter < TIM_COUNTER_VALUE)
            callback_counter++;
        else {
            callback_counter = 0;
            timeout_flag = 1;
        }
    }
}
