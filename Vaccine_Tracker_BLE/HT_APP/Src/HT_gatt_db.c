/*****************************************************************************
* File Name          : HT_gatt_db.c
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : HT GATT db source code. 
*******************************************************************************/

#include "main.h"
#include "ble_const.h"
#include "bluenrg_lp_stack.h"
#include "clock.h"
#include "gp_timer.h"
#include "HT_gatt_db.h"
#include "osal.h"
#include "HT_ble_api.h"

uint16_t frameNotifyServHandle, frameNotifyCharHandle;
uint16_t envSensServHandle, tempCharHandle, logCharHandle, sendFrameCharHandle;

extern uint16_t connection_handle;

/* Client Configuration Characteristics Descriptor Definition: acceleration characteristic*/
BLE_GATT_SRV_CCCD_DECLARE(frame_notify,
                          NUM_LINKS,
                          BLE_GATT_SRV_CCCD_PERM_DEFAULT,
                          BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG);

/* Temperature Notify Characteristics Definition */
static ble_gatt_chr_def_t frame_notify_chars[] = {
    {
        .properties = BLE_GATT_SRV_CHAR_PROP_NOTIFY,
        .permissions = BLE_GATT_SRV_PERM_NONE,
        .min_key_size = BLE_GATT_SRV_MAX_ENCRY_KEY_SIZE,
        .uuid = BLE_UUID_INIT_128(FRAME_NOTIFY_UUID),
        .descrs = {
            .descrs_p = &BLE_GATT_SRV_CCCD_DEF_NAME(frame_notify),
            .descr_count = 1U,
        },
    }};

/* Frame Notify service definition */
static ble_gatt_srv_def_t frame_notify_service = {
    .type = BLE_GATT_SRV_PRIMARY_SRV_TYPE,
    .uuid = BLE_UUID_INIT_128(FRAME_N_UUID),
    .chrs = {
        .chrs_p = frame_notify_chars,
        .chr_count = 1U,
    },
};

/* Temperature characteristic format descriptor definition */
static charactFormat temp_char_format = {
    .format = FORMAT_UINT8,
    .exp = -1,
    .unit = UNIT_UNITLESS,
    .name_space = 0,
    .desc = 0,
};

/* Send frame characteristic format descriptor definition */
static charactFormat send_frame_char_format = {
    .format = FORMAT_UINT8,
    .exp = -1,
    .unit = UNIT_UNITLESS,
    .name_space = 0,
    .desc = 0,
};

/* Environmental temperature characteristic descriptor format definition */
static ble_gatt_val_buffer_def_t env_descr_val_buffers[] =
    {
        {
            .buffer_len = 2,
            .buffer_p = (uint8_t *)&temp_char_format,
        },
        {
            .buffer_len = sizeof(HT_payload),
            .buffer_p = (uint8_t *)&send_frame_char_format,
        }};

/* Environmental characteristic descriptor definition */
static ble_gatt_descr_def_t env_descrs[] =
    {
        {
            .uuid = BLE_UUID_INIT_16(CHAR_FORMAT_DESC_UUID),
            .permissions = BLE_GATT_SRV_PERM_NONE,
            .properties = BLE_GATT_SRV_DESCR_PROP_READ,
            .min_key_size = BLE_GATT_SRV_MAX_ENCRY_KEY_SIZE,
            .val_buffer_p = &env_descr_val_buffers[0], /* temperature char format */
        },
        {
            .uuid = BLE_UUID_INIT_16(CHAR_FORMAT_DESC_UUID),
            .permissions = BLE_GATT_SRV_PERM_NONE,
            .properties = BLE_GATT_SRV_DESCR_PROP_READ,
            .min_key_size = BLE_GATT_SRV_MAX_ENCRY_KEY_SIZE,
            .val_buffer_p = &env_descr_val_buffers[1], /* send frame  char format */
        },
};

/* Environmental characteristic  definition */
static ble_gatt_chr_def_t env_chars[] = {
    {
        /* Temperature Characteristic */
        .properties = BLE_GATT_SRV_CHAR_PROP_READ,
        .permissions = BLE_GATT_SRV_PERM_NONE,
        .min_key_size = BLE_GATT_SRV_MAX_ENCRY_KEY_SIZE,
        .uuid = BLE_UUID_INIT_128(TEMP_CHAR_UUID),
        .descrs = {
            .descrs_p = &env_descrs[0],
            .descr_count = 1,
        },
    },
    {
        /* Send frame Characteristic */
        .properties = BLE_GATT_SRV_CHAR_PROP_READ,
        .permissions = BLE_GATT_SRV_PERM_NONE,
        .min_key_size = BLE_GATT_SRV_MAX_ENCRY_KEY_SIZE,
        .uuid = BLE_UUID_INIT_128(SEND_FRAME_CHAR_UUID),
        .descrs = {
            .descrs_p = &env_descrs[1],
            .descr_count = 1,
        },
    },
};

/* Environmental service definition */
static ble_gatt_srv_def_t env_service = {
    .type = BLE_GATT_SRV_PRIMARY_SRV_TYPE,
    .uuid = BLE_UUID_INIT_128(ENV_SENS_SERVICE_UUID),
    .chrs = {
        .chrs_p = env_chars,
        .chr_count = 2U,
    },
};

tBleStatus Add_Env_Service(void) {
  tBleStatus ret;

  ret = aci_gatt_srv_add_service(&env_service);

  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error while adding Env service.\n");
    return BLE_STATUS_ERROR;
  }

  envSensServHandle = aci_gatt_srv_get_service_handle(&env_service);
  tempCharHandle = aci_gatt_srv_get_char_decl_handle(&env_chars[0]);
  sendFrameCharHandle = aci_gatt_srv_get_char_decl_handle(&env_chars[1]);

  return BLE_STATUS_SUCCESS;
}

tBleStatus Add_Frame_Notify_Service(void) {
  tBleStatus ret;

  ret = aci_gatt_srv_add_service(&frame_notify_service);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error while adding Frame Notify service.\n");
    return BLE_STATUS_ERROR;
  }

  frameNotifyServHandle = aci_gatt_srv_get_service_handle(&frame_notify_service);
  frameNotifyCharHandle = aci_gatt_srv_get_char_decl_handle(&frame_notify_chars[0]);
  
  return BLE_STATUS_SUCCESS;  
}

void aci_gatt_srv_write_event(uint16_t Connection_Handle, uint8_t Resp_Needed, uint16_t Attribute_Handle, uint16_t Data_Length, uint8_t Data[])
{
  uint8_t att_error = BLE_ATT_ERR_NONE;

  if (Resp_Needed == 1U)
    aci_gatt_srv_resp(Connection_Handle, 0, att_error, 0, NULL);
}

/************************ HT_gatt_db.c *****END OF FILE*****************************/
