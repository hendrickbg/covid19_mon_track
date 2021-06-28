/*****************************************************************************
* File Name          : tim.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 28-April-2021
* Description        : Timer config for Vaccine Tracker application.
*******************************************************************************/

#ifndef __TIM_H__
#define __TIM_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Defines ------------------------------------------------------------------*/

/* Compute the prescaler value to have TIM1 counter clock equal to 10000 Hz */
#define PRESCALER_VALUE (uint32_t)((TIM_PERIPHCLK / 10000) - 1)
#define PERIOD_VALUE (50000 - 1)                    /* </ 3s timeout */

#define TIM_COUNTER_VALUE                   2

extern TIM_HandleTypeDef htim1;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void MX_TIM1_Init(void)
 * \brief Init TIM1 peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_TIM1_Init(void);

/*!******************************************************************
 * \fn void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
 * \brief Configures TIM1 interruption priority.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);

#endif /* __TIM_H__ */

/************************ tim.h *****END OF FILE****/
