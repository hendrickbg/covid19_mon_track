
#include "usart.h"

struct __FILE
{
    int handle; /* Add whatever needed */
};
FILE __stdout;
FILE __stdin;
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

UART_HandleTypeDef huart1;

extern uint8_t usart_callback;
extern uint8_t sleeping;
extern volatile uint8_t usart_flag;

void MX_USART1_UART_Init(void) {
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart1) != HAL_OK)
        while (1);

    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        while (1);

    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        while (1);

    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
        while (1);
}

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void) {
    /* System interrupt init*/
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    if (huart->Instance == USART1) {
        /* Peripheral clock enable */
        __HAL_RCC_USART_CLK_ENABLE();

        /**USART1 GPIO Configuration    
        PA9/AF0   ------> USART1_TX
        PA8/AF0   ------> USART1_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

void HT_UART_MspDeInit(void) {
    /* Disable the UART interrupts */
    LL_USART_DisableIT_RXNE(USART1);
    NVIC_DisableIRQ(USART1_IRQn);

    /* Disable the UART peripheral */
    LL_USART_Disable(USART1);

    /* Disable the UART clock */
    __HAL_RCC_USART_FORCE_RESET();
    __HAL_RCC_USART_RELEASE_RESET();

    /* Peripheral clock disable */
    __HAL_RCC_USART_CLK_DISABLE();

    /* Reset the UART pins */
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_OPENDRAIN);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_NO);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
    usart_callback = 1;
    usart_flag = 1;
}

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
