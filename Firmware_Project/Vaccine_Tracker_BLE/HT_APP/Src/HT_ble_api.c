/*****************************************************************************
* File Name          : HT_ble_api.c
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 22-April-2021
* Description        : HT BLE API source code. 
*******************************************************************************/

#include "HT_ble_api.h"
#include "gap_profile.h"

static Advertising_Set_Parameters_t adv_parameters[1];

extern uint8_t connected;
extern uint8_t set_connectable;

uint16_t connection_handle = 0;

extern uint16_t envSensServHandle, tempCharHandle, pressCharHandle, logCharHandle, sendFrameCharHandle;
extern uint16_t frameNotifyServHandle, frameNotifyCharHandle;

uint16_t this_connection_handle, this_attribute_handle, this_data_offset;

extern HT_Event_Type event_type;

extern volatile uint8_t deepSleepModeFlag;
extern uint8_t wkp_flag;

void hci_le_enhanced_connection_complete_event(uint8_t Status, uint16_t Connection_Handle, uint8_t Role, 
                            uint8_t Peer_Address_Type, uint8_t Peer_Address[6], 
                            uint8_t Local_Resolvable_Private_Address[6], 
                            uint8_t Peer_Resolvable_Private_Address[6],
                            uint16_t Conn_Interval, uint16_t Conn_Latency, 
                            uint16_t Supervision_Timeout, uint8_t Master_Clock_Accuracy)
{

    hci_le_connection_complete_event(Status, Connection_Handle, Role, Peer_Address_Type, Peer_Address, Conn_Interval,
                                     Conn_Latency, Supervision_Timeout, Master_Clock_Accuracy);
}

void hci_le_connection_complete_event(uint8_t Status, uint16_t Connection_Handle, uint8_t Role, uint8_t Peer_Address_Type, uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval, uint16_t Conn_Latency, uint16_t Supervision_Timeout, uint8_t Master_Clock_Accuracy)
{
    printf("Connected!\n");

    if(deepSleepModeFlag)  {
        wkp_flag = 1;
        HT_APP_WakeupHandle();
        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    }

    connected = 1;
    set_connectable = FALSE;
    connection_handle = Connection_Handle;
}

void hci_disconnection_complete_event(uint8_t Status, uint16_t Connection_Handle, uint8_t Reason) {
    
    printf("Disconnected\n");
    connected = 0;
    /* Make the device connectable again. */
    set_connectable = TRUE;
    connection_handle = 0;
} 

void hci_le_connection_update_complete_event(uint8_t Status, uint16_t Connection_Handle, uint16_t Conn_Interval,
                                             uint16_t Conn_Latency,
                                             uint16_t Supervision_Timeout)
{
    printf("hci_le_connection_update_complete_event; %d\r\n", Conn_Interval);
}

void HT_BLE_BleConfig(void) {
    uint8_t ret;
    uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
    uint8_t device_name[] = {"Vaccine"};
    static uint8_t adv_data[] = {0x02, AD_TYPE_FLAGS, FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE | FLAG_BIT_BR_EDR_NOT_SUPPORTED,
                                 8, AD_TYPE_COMPLETE_LOCAL_NAME, 'V', 'a', 'c', 'c', 'i', 'n', 'e'};

    /* Set the TX power 0 dBm */
    aci_hal_set_tx_power_level(0, 25);

    /* GATT Init */
    ret = aci_gatt_srv_init();
    if (ret != BLE_STATUS_SUCCESS)
        while(1);

    /* GAP Init */
    ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, STATIC_RANDOM_ADDR, &service_handle, &dev_name_char_handle, &appearance_char_handle);
    if (ret != BLE_STATUS_SUCCESS)
        while(1);

    /* Update device name */
    Gap_profile_set_dev_name(0, strlen((char *)device_name), device_name);

    ret = aci_gap_set_authentication_requirement(BONDING,
                                                 MITM_PROTECTION_REQUIRED,
                                                 SC_IS_NOT_SUPPORTED,
                                                 KEYPRESS_IS_NOT_SUPPORTED,
                                                 7,
                                                 16,
                                                 USE_FIXED_PIN_FOR_PAIRING,
                                                 123456);
    if (ret != BLE_STATUS_SUCCESS)
        while(1);

    ret = aci_gap_set_advertising_configuration(0, GAP_MODE_GENERAL_DISCOVERABLE,
                        ADV_PROP_CONNECTABLE | ADV_PROP_SCANNABLE | ADV_PROP_LEGACY,
                        (ADV_INTERVAL_SLOW_MS * 1000) / 625, (ADV_INTERVAL_SLOW_MS * 1000) / 625,
                        ADV_CH_ALL,
                        0, NULL,
                        ADV_NO_WHITE_LIST_USE,
                        0,         /* 0 dBm */
                        LE_1M_PHY, /* Primary advertising PHY */
                        0,         /* 0 skips */
                        LE_1M_PHY, /* Secondary advertising PHY. Not used with legacy advertising. */
                        0,         /* SID */
                        0 /* No scan request notifications */);

    ret = aci_gap_set_advertising_data(0, ADV_COMPLETE_DATA, sizeof(adv_data), adv_data);

    ret = Add_Env_Service();
    if(ret == BLE_STATUS_SUCCESS) 
        printf("Environmental service added successfully.\n");
    else 
        printf("Error while adding Environmental service: 0x%04x\r\n", ret);
    
    ret = Add_Frame_Notify_Service();
    if(ret == BLE_STATUS_SUCCESS) 
        printf("Frame Notify notify service added successfully.\n");
    else 
        printf("Error while adding Frame Notify service: 0x%02x\r\n", ret);
    
}

void HT_BLE_SendNotifyAlert(HT_payload *payload) {
    tBleStatus ret;

    ret = aci_gatt_srv_notify(connection_handle, (frameNotifyCharHandle + 1), 0, 
        sizeof(HT_payload), (uint8_t *)payload);

    if (ret != BLE_STATUS_SUCCESS)
        printf("Error while updating Frame Notify characteristic: 0x%02X\n",ret);
    
}

void HT_BLE_DisableConnectableDevice(void) {
    uint8_t ret;

    adv_parameters[0].Advertising_Handle = 0;
    adv_parameters[0].Duration = 0;
    adv_parameters[0].Max_Extended_Advertising_Events = 0;

    ret = aci_gap_set_advertising_enable(0, 1, adv_parameters);

    if (ret != BLE_STATUS_SUCCESS)
        printf("aci_gap_set_advertising_enable() failed: 0x%02x\r\n", ret);
}

void HT_BLE_SetDeviceConnectable(void)
{
    uint8_t ret;

    adv_parameters[0].Advertising_Handle = 0;
    adv_parameters[0].Duration = 0;
    adv_parameters[0].Max_Extended_Advertising_Events = 0;

    ret = aci_gap_set_advertising_enable(ENABLE, 1, adv_parameters);

    if (ret != BLE_STATUS_SUCCESS)
        printf("aci_gap_set_advertising_enable() failed: 0x%02x\r\n", ret);
}

void aci_gatt_srv_read_event(uint16_t Connection_Handle, uint16_t Attribute_Handle, uint16_t Data_Offset) {
    uint8_t data[2];
    int32_t tmp;

    HT_payload payload = {0};
    
    float fdata;
    
    if(Attribute_Handle == tempCharHandle + 1) {
        
        fdata = HT_APP_GetTemperature();

        tmp = (int32_t)fdata;
        tmp = (fdata - tmp)*100;
        
        data[0] = (uint8_t)(tmp);
        data[1] = (uint8_t)(fdata);

        aci_gatt_srv_resp(Connection_Handle, Attribute_Handle, Data_Offset, sizeof(data), data);
        
    } else if(Attribute_Handle == (sendFrameCharHandle + 1)) {
        
        event_type = HT_BUTTON_EVENT;

		HT_APP_SetNewState(HT_SM_BUTTON_HANDLER);

        HT_APP_TemparatureHandlerState();
        HT_APP_GetInfos(&payload);

        aci_gatt_srv_resp(Connection_Handle, Attribute_Handle, Data_Offset, sizeof(HT_payload), (uint8_t *)&payload);
    }
}

/************************ HT_ble_api.c *****END OF FILE*****************************/
