/*****************************************************************************
* File Name          : HT_ble_api.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : Bluetooth Low Energy API for Vaccine Tracker application. 
*******************************************************************************/

#ifndef __HT_BLE_API__
#define __HT_BLE_API__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"
#include "HT_app.h"
#include "HT_gatt_db.h"

/* Defines  ------------------------------------------------------------------*/

/* Advertising interval */
#define ADV_INTERVAL_FAST_MS  100            
#define ADV_INTERVAL_SLOW_MS  1000

/* Vaccine tracker Bluetooth characteristics. */
#define SEND_FRAME_CHAR_UUID    0x19,0x81,0xd5,0xa5,0x02,0x00,0xe3,0xa9,0xe2,0x11,0x77,0xe4,0x20,0x55,0x2e,0xa3
#define LOG_CHAR_UUID 			0x19,0x83,0xd5,0xa5,0x02,0x00,0xe3,0xa9,0xe2,0x11,0x77,0xe4,0x20,0x55,0x2e,0xa3
#define FRAME_N_UUID            0x20,0x01,0xd5,0xa5,0x02,0x00,0xfc,0x8f,0xe1,0x11,0x4a,0xcf,0xa0,0x78,0x3e,0xe2
#define FRAME_NOTIFY_UUID       0x19,0x95,0xd5,0xa5,0x02,0x00,0xfc,0x8f,0xe1,0x11,0x4a,0xcf,0xa0,0x78,0x3e,0xe2
#define TEMP_CHAR_UUID          0x19,0x96,0xd5,0xa5,0x02,0x00,0xe3,0xa9,0xe2,0x11,0x77,0xe4,0x20,0x55,0x2e,0xa3
#define GEO_CHAR_UUID           0x20,0x17,0xd5,0xa5,0x02,0x00,0xe3,0xa9,0xe2,0x11,0x77,0xe4,0x20,0x55,0x2e,0xa3
#define ENV_SENS_SERVICE_UUID   0x20,0x16,0xd5,0xa5,0x02,0x00,0xd0,0x82,0xe2,0x11,0x77,0xe4,0x40,0x1a,0x82,0x42

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_BLE_SetDeviceConnectable(void)
 * \brief Set connectable mode.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BLE_SetDeviceConnectable(void);

/*!******************************************************************
 * \fn void HT_BLE_BleConfig(void)
 * \brief Init bluetooth stack.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BLE_BleConfig(void);

/*!******************************************************************
 * \fn void HT_BLE_ConfigureDeviceAdvertising(uint16_t adv_interval)
 * \brief Configure bluetooth advertising interval.
 *
 * \param[in]  uint16_t adv_interval             Advertising interval
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BLE_ConfigureDeviceAdvertising(uint16_t adv_interval);

/*!******************************************************************
 * \fn void HT_BLE_SendNotifyAlert(HT_payload *payload);
 * \brief Configure bluetooth advertising interval.
 *
 * \param[in]  HT_payload *payload                     Payload to be sent.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BLE_SendNotifyAlert(HT_payload *payload);

/*!******************************************************************
 * \fn void HT_BLE_DisableConnectableDevice(void)
 * \brief Disable connectable mode.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BLE_DisableConnectableDevice(void);

#endif /* __HT_BLE_API__ */

/************************ HT_ble_api.h *****END OF FILE*****************************/
