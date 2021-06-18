/*****************************************************************************
* File Name          : main.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : Main header file for Vaccine Tracker application.
*******************************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes  ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "bluenrg_lp_it.h"
#include "ble_const.h"
#include "bluenrg_lp_stack.h"
#include "bluenrg_lp_hal_power_manager.h"
#include "bluenrg_lp_hal_vtimer.h"

#include "vaccine_config.h"
#include "HT_gatt_db.h"
#include "bleplat.h"
#include "nvm_db.h"

#include "bluenrg_lp_hal.h"
#include "bluenrg_lp_hal_rtc.h"
#include "bluenrg_lp_hal_rtc_ex.h"
#include "bluenrg_lp_ll_rtc.h"

#include "bluenrg_lp_hal_pwr.h"
#include "bluenrg_lp_hal_cortex.h"
#include "bluenrg_lp_hal_rcc.h"
#include "bluenrg_lp_ll_utils.h"
#include "bluenrg_lp_hal_gpio.h"
#include "bluenrg_lp_ll_gpio.h"
#include "bluenrg_lp_ll_adc.h"
#include "bluenrg_lp_hal_uart.h"
#include "bluenrg_lp_hal_uart_ex.h"
#include "bluenrg_lp_ll_usart.h"
#include "bluenrg_lp_hal_exti.h"
#include "bluenrg_lp_hal_tim.h"
#include "bluenrg_lp_hal_spi.h"
#include "bluenrg_lp_hal_spi_ex.h"
#include "bluenrg_lp_ll_dma.h"

#include "rtc.h"
#include "dma.h"
#include "adc.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "spi.h"

#include "HT_app.h"

/* Defines  ------------------------------------------------------------------*/

/* Display interface */
#define DC_Pin GPIO_PIN_8
#define DC_GPIO_Port GPIOB
#define BUSY_Pin GPIO_PIN_2
#define BUSY_GPIO_Port GPIOB //VAI SER PB6
#define RST_Pin GPIO_PIN_5
#define RST_GPIO_Port GPIOB  //VAI SER O PB10
#define SPI_CS_Pin GPIO_PIN_11 
#define SPI_CS_GPIO_Port GPIOA


/* Functions  ------------------------------------------------------------------*/
void ModulesTick(void);
void ModulesInit(void);

#endif /* __MAIN_H__ */

/************************ main.h *****END OF FILE*****************************/
