/*****************************************************************************
* File Name          : HT_app.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : Vaccine Tracker application main file. 
*******************************************************************************/

#ifndef __HT_API__
#define __HT_API__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "epd2in9.h"
#include "epdif.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "HT_sensor.h"

/* Defines  ------------------------------------------------------------------*/
#define HT_SIGFOX_RCZ 				2				/* </ Sigfox region (RC Brazil). */

#define ERR_NONE 					0				/* </ Error none value */
#define USART_BUFFER_SIZE 			3				/* </ Sigfox region (RC Brazil). */

#define RTC_WAKEUP_TIMEOUT  		720 //s 	/* </ Wakeup timer. */
#define REPETITION_COUNTER 			3 				

#define SEND_FRAME_TIME 			5 //5*12 = 1h	/* </ 1 frame per hour. */

#define MAX_TEMPERATURE 			8				/* </ Max temperature range. */
#define MIN_TEMPERATURE 			2				/* </ Min temperature range. */

#define TOLERANCE					0.2

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum HT_Fsm
 * \brief Vaccine tracker FSM states.
 */
typedef enum {	
	HT_SM_DEEP_SLEEP,								/* </ Deep sleep state. */
	HT_SM_TEMPERATURE_HANDLER,						/* </ Temperature handler state. */
	HT_SM_CHECK_TIMER,								/* </ Check timer state. */
	HT_SM_BUTTON_HANDLER,							/* </ Button handler state. */
	HT_SM_SEAL_HANDLER,								/* </ Seal handler state. */
	HT_SM_CALIBRATION_PROCESS,						/* </ Calibration state. */
	HT_SM_SEND_FRAME,								/* </ Send frame state. */
	HT_SM_RESET_SIGFOX_MOD							/* </ Reset HT32SX state. */
} HT_Fsm;

/**
 * \enum HT_Event_Type
 * \brief Specifies the event which genereted the alert.
 */
typedef enum {
	HT_BUTTON_EVENT = 0,							/* </ Button event. */
	HT_SEAL_EVENT,								/* </ Seal state. */
	HT_TEMPERATURE_EVENT,						/* </ Temperature event. */
	HT_PERIODIC_EVENT							/* </ Periodic event. */
} HT_Event_Type;		

/**
 * \enum HT_Seal_State
 * \brief Specifies the seal state.
 */
typedef enum {
	HT_SEAL_CLOSED = 0,							/* </ Seal open. */
	HT_SEAL_OPEN								/* </ Periodic closed. */
} HT_Seal_State;

/**
 * \struct HT_payload
 * \brief Payload that will be sent to the Sigfox Network.
 */
typedef struct {
	uint8_t event_type;
	int16_t max_temperature;
	int16_t min_temperature;
	int16_t current_temperature;
	uint8_t battery_level;
	uint8_t seal_state;
} __attribute__((packed)) HT_payload; /* Sigfox payload */

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_app(void)
 * \brief Vaccine tracker main function.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_app(void);

/*!******************************************************************
 * \fn void HT_APP_fsm(void)
 * \brief Vaccine tracker finite state machine.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_fsm(void);

/*!******************************************************************
 * \fn void HT_APP_SendFrameState(HT_payload *payload)
 * \brief Send frame state. Send a frame to the Sigfox network with the temperature values
 * read from ADC.
 *
 * \param[in]  HT_payload *payload			Payload buffer.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_SendFrameState(HT_payload *payload);

/*!******************************************************************
 * \fn void HT_APP_TemparatureHandlerState(void)
 * \brief Read temperature state. Read the NTC value from ADC peripheral. 
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_TemparatureHandlerState(void);

/*!******************************************************************
 * \fn void HT_APP_ButtonHandlerState(void)
 * \brief Button handler state. Handle a button request from BLE (from bluetooth). 
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_ButtonHandlerState(void);

/*!******************************************************************
 * \fn void HT_APP_DeepSleepState(void)
 * \brief Deep sleep mode state.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_DeepSleepState(void);

/*!******************************************************************
 * \fn void HT_APP_SealHandlerState(void)
 * \brief Manages the breaking of the seal (GPIO EXTI).
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_SealHandlerState(void);

/*!******************************************************************
 * \fn void HT_APP_InitHT32SX(void)
 * \brief Do a reset in the HT32SX to pair the two systems.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_InitHT32SX(void);

/*!******************************************************************
 * \fn void HT_APP_SendATCommand(char *cmd)
 * \brief Send an AT Command to the iMCP HT32SX. It will wait until receives an answer.
 *
 * \param[in]  char *cmd			Command string.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_SendATCommand(char *cmd);

/*!******************************************************************
 * \fn void HT_APP_EnableRTCWkp(uint32_t seconds)
 * \brief Enable the RTC wake up.
 *
 * \param[in]  uint32_t seconds			RTC wake up timer in seconds.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_EnableRTCWkp(uint32_t seconds);

/*!******************************************************************
 * \fn void HT_APP_WaitForResponse(void)
 * \brief Wait until receives a response from HT32SX, after a reset. 
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_WaitForResponse(void);

/*!******************************************************************
 * \fn extern void HT_APP_SetNewState(HT_Fsm new_state)
 * \brief Set a new state for the FSM.
 *
 * \param[in] HT_Fsm new_state			New state.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void HT_APP_SetNewState(HT_Fsm new_state);

/*!******************************************************************
 * \fn void HT_APP_InitTimeout(void)
 * \brief initiates a timeout for the HT32SX response.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_InitTimeout(void);

/*!******************************************************************
 * \fn void HT_APP_InitTimeout(HT_payload *payload)
 * \brief Get the updated information about the environment before send a frame to the Sigfox network or
 * bluetooth. 
 *
 * \param[in] none
 * \param[out] HT_payload *payload			Payload buffer.
 *
 * \retval none
 *******************************************************************/
void HT_APP_GetInfos(HT_payload *payload);

/*!******************************************************************
 * \fn void HT_APP_CloseSigfoxApi(void)
 * \brief Close the Sigfox stack (AT+CLOSE).
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_CloseSigfoxApi(void);

/*!******************************************************************
 * \fn void HT_APP_ResetInfo(void)
 * \brief Reset the context information after update these values in the cloud. 
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_ResetInfo(void);

/*!******************************************************************
 * \fn void HT_APP_UpdateDisplay(void)
 * \brief Update the e-paper display information after read the temperature sensor.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_UpdateDisplay(void);

/*!******************************************************************
 * \fn void HT_APP_GetTemperature(void)
 * \brief Get the temperature value.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
float HT_APP_GetTemperature(void);

/*!******************************************************************
 * \fn void HT_APP_ReadTemperatureSensor(void)
 * \brief Read the temperature sensor.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_ReadTemperatureSensor(uint8_t ble_resp);

/*!******************************************************************
 * \fn void HT_APP_ResetSigfoxMod(void)
 * \brief Resets the Sigfox module (HT32SX) with a falling edge in the PA14.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_ResetSigfoxMod(void);

/*!******************************************************************
 * \fn void HT_APP_WakeupHandle(void)
 * \brief Reconfigure all needed peripherals after deep sleep mode.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_WakeupHandle(void);

/*!******************************************************************
 * \fn void HT_APP_ConfigWakeupIO(void)
 * \brief Configure the wakeup IOs. 
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_APP_ConfigWakeupIO(void);

#endif /* __HT_API__ */

/************************ HT_app.h *****END OF FILE*****************************/
