
#include "adc.h"

ADC_HandleTypeDef hadc;
ADC_ConfigChannelTypeDef xChannel;

static uint16_t dma_buffer[BUFFER_SIZE];

void MX_ADC_Init(void) {

    LL_ADC_MspInit();

    /* Peripheral clock enable */
    LL_APB1_EnableClock(LL_APB1_PERIPH_ADCDIG | LL_APB1_PERIPH_ADCANA);

    /* This function must not be called on QFN32 package */
    LL_ADC_LDOEnable(ADC);

    /* Enable the ADC */
    LL_ADC_Enable(ADC);

    /* Configure the sample rate */
    LL_ADC_SetSampleRate(ADC, USER_SAMPLERATE);

    /* Configure the Down Sampler data width and ratio */
    LL_ADC_ConfigureDSDataOutput(ADC, USER_DATAWIDTH, USER_RATIO);

    /* Configure the operation mode as ADC mode (static/low frequency signal) */
    LL_ADC_SetADCMode(ADC, LL_ADC_OP_MODE_ADC);

    /* Set the length of the conversion sequence as 2 */
    LL_ADC_SetSequenceLength(ADC, LL_ADC_SEQ_LEN_02);

    /* Set the 1st entry of the input sequence as VINP0 */
    LL_ADC_SetChannelSeq0(ADC, LL_ADC_CH_VINP0_TO_SINGLE_POSITIVE_INPUT);
    LL_ADC_SetVoltageRangeSingleVinp0(ADC, LL_ADC_VIN_RANGE_3V6);
    LL_ADC_SetCalibPoint1Gain(ADC, LL_ADC_DEFAULT_RANGE_VALUE_3V6);

    /* Set the 2st entry of the input sequence as battery level detector */
    LL_ADC_SetChannelSeq1(ADC, LL_ADC_CH_BATTERY_LEVEL_DETECTOR);
}

void HT_ADC_DeInit(void) {
    LL_APB1_DisableClock(LL_APB1_PERIPH_ADCDIG | LL_APB1_PERIPH_ADCANA);

    LL_ADC_Disable(ADC);
    LL_ADC_LDODisable(ADC);
}

void LL_ADC_MspInit(void) {
    LL_GPIO_InitTypeDef gpioinitstruct = {0};
    LL_GPIO_StructInit(&gpioinitstruct);

    /* Initialization of the ADC pins */
    LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOB);
    gpioinitstruct.Pin = LL_GPIO_PIN_3;
    gpioinitstruct.Mode = LL_GPIO_MODE_ANALOG;
    gpioinitstruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    gpioinitstruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpioinitstruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &gpioinitstruct);
}

void HT_ADC_ConfigDMA(void) {
    /* Set DMA transfer addresses of source and destination */
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1,
                           LL_ADC_GetOutputDataRegDS(ADC),
                           (uint32_t)&dma_buffer,
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    /* Set DMA transfer size */
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, BUFFER_SIZE);

    /* Enable DMA transfer complete/error interrupts */
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

    LL_ADC_DMAModeDSEnable(ADC);
}

void HT_ADC_RestartDmaChannel(void) {

    /* Restart DMA channel */
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&dma_buffer);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, BUFFER_SIZE);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

void HT_ADC_ReadADC(uint32_t *vout, uint32_t *battery_lvl) {
    
    HT_ADC_RestartDmaChannel();
    HT_ADC_ConfigDMA();
    
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    /* Start ADC conversion */
    LL_ADC_StartConversion(ADC);

    while(!LL_DMA_IsActiveFlag_TC1(DMA1));

    *vout = LL_ADC_GetADCConvertedValueSingle(ADC, dma_buffer[0], LL_ADC_VIN_RANGE_3V6, USER_DATAWIDTH);
    *battery_lvl = LL_ADC_GetADCConvertedValueBatt(ADC, dma_buffer[1], USER_DATAWIDTH);

    LL_DMA_ClearFlag_TC1(DMA1);
    LL_ADC_ClearFlag_EOS(ADC);
}

void HT_ADC_ReadFloatAdc(float *v_out, float *batt_lvl) {
    uint32_t vout = 0;
    float f_vout;
    uint32_t battery_lvl = 0;
    float f_battery_lvl;
    
    HT_ADC_ReadADC(&vout, &battery_lvl);
    
    f_vout = (((float)vout)/1000);
    f_battery_lvl = (((float)battery_lvl)/1000);

    *v_out = f_vout;
    *batt_lvl = f_battery_lvl;
}
