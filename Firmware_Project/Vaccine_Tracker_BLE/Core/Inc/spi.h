/*****************************************************************************
* File Name          : spi.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 25-April-2021
* Description        : SPI config for Vaccine Tracker application.
*******************************************************************************/

#ifndef __SPI_H__
#define __SPI_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern SPI_HandleTypeDef hspi1;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void MX_SPI1_Init(void)
 * \brief Init SPI1 peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_SPI1_Init(void);

#endif /* __SPI_H__ */

/************************ spi.h *****END OF FILE****/
