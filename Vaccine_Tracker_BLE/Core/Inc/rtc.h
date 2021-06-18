/*****************************************************************************
* File Name          : rtc.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 25-April-2021
* Description        : RTC config for Vaccine Tracker application.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern RTC_HandleTypeDef hrtc;

/* Defines ------------------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0x0FF

/*!******************************************************************
 * \fn void MX_RTC_Init(void)
 * \brief Init RTC peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_RTC_Init(void);

#endif /* __RTC_H__ */

/************************ rtc.h *****END OF FILE****/
