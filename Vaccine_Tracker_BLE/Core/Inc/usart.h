/*****************************************************************************
* File Name          : uart.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 28-April-2021
* Description        : USART config for Vaccine Tracker application.
*******************************************************************************/

#ifndef __USART_H__
#define __USART_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "HT_app.h"

extern UART_HandleTypeDef huart1;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void MX_USART1_UART_Init(void)
 * \brief Init USART1 peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_USART1_UART_Init(void);

/*!******************************************************************
 * \fn void BSP_COM_DeInit(void)
 * \brief DeInit USART1 peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_UART_MspDeInit(void);

#endif /* __USART_H__ */

/************************ usart.h *****END OF FILE****/
