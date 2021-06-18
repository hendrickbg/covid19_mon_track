/*****************************************************************************
* File Name          : HT_sensor.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : Temperature sensor (NTC) API for Vaccine Tracker applicatin.
*******************************************************************************/

#ifndef __HT_SENSOR_H__
#define __HT_SENSOR_H__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"

/* Defines  ------------------------------------------------------------------*/
#define VSUPPLY     3.3
#define R_10k       9800//10000     /* </ 10k resistor measured resistance in Ohms (other element in the voltage divider) */
#define B_PARAM     4300            /* </ B-coefficient of the thermistor */
#define T0          298.15          /* </ 25°C in Kelvin */
#define T25         25
#define TK          273.15 

/* y = -7.7677x2 + 3.8371x + 37.653 */
#define a2                  -7.7677         
#define a1                  3.8371
#define a0                  37.653

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn float HT_SENSOR_CalculateTemperature(float v_out)
 * \brief Calculate temperature sensor value.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval Temperature absolute value.
 *******************************************************************/
float HT_SENSOR_CalculateTemperature(float v_out);

#endif /* __HT_SENSOR_H__ */

/************************ HT_sensor.h *****END OF FILE*****************************/
