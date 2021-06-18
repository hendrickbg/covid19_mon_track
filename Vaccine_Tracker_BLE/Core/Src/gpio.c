
#include "gpio.h"

EXTI_HandleTypeDef HEXTI_Seal;

extern volatile uint8_t gpio_callback;
extern HT_Fsm fsm_state;
extern volatile uint8_t deepSleepModeFlag;

void MX_GPIO_Init(void) {
    GPIO_ConfigUserLed();
    GPIO_ConfigSfxWakeUpPin();
    GPIO_ConfigSfxRstPin();
    GPIO_ConfigDisplayPins();
    GPIO_ConfigSealPin();
}

void MX_GPIO_LP_Init(void) {
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO0);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO1);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO2);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO3);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO4);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO5);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO6);
    LL_PWR_EnablePUA(LL_PWR_PUPD_IO7);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO8);
    LL_PWR_EnablePUA(LL_PWR_PUPD_IO9);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO10);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO11);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO12);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO13);
    LL_PWR_EnablePUA(LL_PWR_PUPD_IO14);
    LL_PWR_EnablePDA(LL_PWR_PUPD_IO15);

    LL_PWR_EnablePUB(LL_PWR_PUPD_IO0);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO1);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO2);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO3);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO4);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO5);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO6);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO7);
    LL_PWR_EnablePUB(LL_PWR_PUPD_IO8);
    LL_PWR_EnablePUB(LL_PWR_PUPD_IO9);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO10);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO11);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO12);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO13);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO14);
    LL_PWR_EnablePDB(LL_PWR_PUPD_IO15);
}

void GPIO_ConfigUserLed(void) {

    /* It is going to be PA5 */
    LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_6);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_NO);
}

void GPIO_ConfigSfxWakeUpPin(void) {

    /* PB7 Configured as Output*/
    LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOB);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN);
    GPIOB->BSRR = 1 << 23; /* </ Set GPIO to low level */
}

void GPIO_ConfigSfxRstPin(void) {

    /* It is going to be ble_pb1*/
    LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_14);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_14, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);
    GPIOA->BSRR = 1 << 14; /* </ Set GPIO to high level */
}

void GPIO_ConfigDisplayPins(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, BUSY_Pin | RST_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : DC_Pin */
    GPIO_InitStruct.Pin = DC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DC_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : Busy and RST Pin */
    GPIO_InitStruct.Pin = BUSY_Pin | RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI_CS_Pin */
    GPIO_InitStruct.Pin = SPI_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : Display VDD switch transistor PNP */ 
    LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
    GPIOA->BSRR = 1 << 7;
}

void GPIO_ConfigSealPin(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    EXTI_ConfigTypeDef EXTI_Config_InitStructure;

    /* PB4 Seal*/
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    EXTI_Config_InitStructure.Line =    EXTI_LINE_PB4;
    EXTI_Config_InitStructure.Trigger = EXTI_TRIGGER_FALLING_EDGE;
    EXTI_Config_InitStructure.Type =    EXTI_TYPE_EDGE;
    
    HAL_EXTI_SetConfigLine(&HEXTI_Seal, &EXTI_Config_InitStructure);
    HAL_EXTI_RegisterCallback(&HEXTI_Seal, HAL_EXTI_COMMON_CB_ID, GPIOB_IRQn_Handler);
    HAL_EXTI_Cmd(&HEXTI_Seal , ENABLE);
    
    HAL_EXTI_ClearPending(&HEXTI_Seal);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_SetPriority(GPIOB_IRQn, 0);
    NVIC_EnableIRQ(GPIOB_IRQn);

    /* Configure NVIC for SysTick_IRQn */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0);
}

void GPIOB_IRQn_Handler(uint32_t Line) {

    if(deepSleepModeFlag)
        HT_APP_WakeupHandle();

    fsm_state = HT_SM_SEAL_HANDLER;
}
