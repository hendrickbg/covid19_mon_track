
#include "dma.h"

void MX_DMA_Init(void) {

    /* Init with LL driver */
    /* DMA controller clock enable */
    LL_AHB_EnableClock(LL_AHB_PERIPH_DMA);

    /* Configure DMA request MEMTOMEM_DMA1_Channel1 */

    /* Set the DMA channel 1 with the ADC Down Sampler output */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_ADC_DS);

    /* Set transfer direction from ADC DS peripheral to RAM memory */
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    /* Set priority level */
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);

    /* Set DMA mode */
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);

    /* Set peripheral increment mode to no increment */
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

    /* Set memory increment mode to increment */
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

    /* Set peripheral data width to 16-bit */
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);

    /* Set memory data width to 16-bit */
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
}

void HT_DMA_DeInit(void) {
    LL_APB1_DisableClock(LL_AHB_PERIPH_DMA);

    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
}
