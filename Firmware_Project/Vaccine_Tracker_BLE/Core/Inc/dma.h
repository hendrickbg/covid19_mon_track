/*****************************************************************************
* File Name          : dma.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : DMA config for Vaccine Tracker application.
*******************************************************************************/

#ifndef __DMA_H__
#define __DMA_H__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void MX_DMA_Init(void)
 * \brief Init DMA peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_DMA_Init(void);

void HT_DMA_DeInit(void);

#endif /* __DMA_H__ */

/************************ dma.h *****END OF FILE*****************************/
