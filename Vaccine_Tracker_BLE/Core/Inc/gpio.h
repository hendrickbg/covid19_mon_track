/*****************************************************************************
* File Name          : gpio.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : GPIO config for Vaccine Tracker application.
*******************************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"

/* Defines  ------------------------------------------------------------------*/
#define SEAL_Pin GPIO_PIN_4

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void MX_GPIO_Init(void)
 * \brief Init all needed BLE GPIOs.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_GPIO_Init(void);

/*!******************************************************************
 * \fn void MX_GPIO_LP_Init(void)
 * \brief Set all unused GPIOs to low power mode.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_GPIO_LP_Init(void);

/*!******************************************************************
 * \fn void GPIO_ConfigSealPin(void)
 * \brief Configure seal pin.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void GPIO_ConfigSealPin(void);

/*!******************************************************************
 * \fn void GPIOB_IRQn_Handler(uint32_t Line)
 * \brief Seal pin interruption callback.
 *
 * \param[in]  uint32_t Line                EXTI line.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void GPIOB_IRQn_Handler(uint32_t Line);

/*!******************************************************************
 * \fn void GPIO_ConfigDisplayPins(void)
 * \brief Configures ePaper display pins.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void GPIO_ConfigDisplayPins(void);

/*!******************************************************************
 * \fn void GPIO_ConfigSfxRstPin(void)
 * \brief Configures HT32SX reset pin.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void GPIO_ConfigSfxRstPin(void);

/*!******************************************************************
 * \fn void GPIO_ConfigSfxWakeUpPin(void)
 * \brief Configures HT32SX wakeup pin.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void GPIO_ConfigSfxWakeUpPin(void);

/*!******************************************************************
 * \fn void GPIO_ConfigUserLed(void)
 * \brief Configures user led.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void GPIO_ConfigUserLed(void);

#endif /* __GPIO_H__ */

/************************ gpio.h *****END OF FILE*****************************/
