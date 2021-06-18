/*****************************************************************************
* File Name          : HT_sensor.c
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : HT sensor source code. 
*******************************************************************************/

#include "HT_sensor.h"

float HT_SENSOR_CalculateTemperature(float v_out) {
    double temp_c;

    temp_c = a2*(v_out*v_out) + a1*v_out + a0;

    return (float)temp_c;
}

/************************ HT_sensor.c *****END OF FILE*****************************/
