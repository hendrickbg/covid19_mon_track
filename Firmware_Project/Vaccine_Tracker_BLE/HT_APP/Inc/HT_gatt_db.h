/*****************************************************************************
* File Name          : gatt_db.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 20-April-2021
* Description        : Functions to build GATT DB and handle GATT events.
*******************************************************************************/

#ifndef __HT_GATT_DB_H__
#define __HT_GATT_DB_H__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn tBleStatus Add_Env_Service(void)
 * \brief Add environment service.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
tBleStatus Add_Env_Service(void);

/*!******************************************************************
 * \fn tBleStatus Add_Frame_Notify_Service(void)
 * \brief Add notify service.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
tBleStatus Add_Frame_Notify_Service(void);

#endif /* __GATT_DB_H__ */

/************************ HT_gatt_db.h *****END OF FILE*****************************/
