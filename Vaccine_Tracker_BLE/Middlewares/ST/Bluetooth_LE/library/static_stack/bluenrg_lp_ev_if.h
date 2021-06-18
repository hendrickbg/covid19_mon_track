

/**
  ******************************************************************************
  * @file    bluenrg_lp_ev_if.h
  * @author  AMS - RF Application team
  * @date    01 June 2020
  * @brief   Autogenerated files, do not edit!!
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef _BLUENRG_LP_EV_IF_H_
#define _BLUENRG_LP_EV_IF_H_

#define HCI_DISCONNECTION_COMPLETE_EVENT                      0
#define HCI_ENCRYPTION_CHANGE_EVENT                           1
#define HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT    2
#define HCI_HARDWARE_ERROR_EVENT                              3
#define HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT                 4
#define HCI_DATA_BUFFER_OVERFLOW_EVENT                        5
#define HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT             6
#define HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT       7
#define ACI_HAL_END_OF_RADIO_ACTIVITY_EVENT                   8
#define ACI_HAL_SCAN_REQ_REPORT_EVENT                         9
#define ACI_HAL_FW_ERROR_EVENT                               10
#define ACI_HAL_ADV_SCAN_RESP_DATA_UPDATE_EVENT              11
#define ACI_GAP_LIMITED_DISCOVERABLE_EVENT                   12
#define ACI_GAP_PAIRING_COMPLETE_EVENT                       13
#define ACI_GAP_PASS_KEY_REQ_EVENT                           14
#define ACI_GAP_AUTHORIZATION_REQ_EVENT                      15
#define ACI_GAP_SLAVE_SECURITY_INITIATED_EVENT               16
#define ACI_GAP_BOND_LOST_EVENT                              17
#define ACI_GAP_PROC_COMPLETE_EVENT                          18
#define ACI_GAP_ADDR_NOT_RESOLVED_EVENT                      19
#define ACI_GAP_NUMERIC_COMPARISON_VALUE_EVENT               20
#define ACI_GAP_KEYPRESS_NOTIFICATION_EVENT                  21
#define ACI_L2CAP_CONNECTION_UPDATE_RESP_EVENT               22
#define ACI_L2CAP_PROC_TIMEOUT_EVENT                         23
#define ACI_L2CAP_CONNECTION_UPDATE_REQ_EVENT                24
#define ACI_L2CAP_CFC_CONNECTION_EVENT                       25
#define ACI_L2CAP_DISCONNECTION_COMPLETE_EVENT               26
#define ACI_L2CAP_FLOW_CONTROL_CREDIT_EVENT                  27
#define ACI_L2CAP_COMMAND_REJECT_EVENT                       28
#define ACI_L2CAP_SDU_DATA_TX_EVENT                          29
#define ACI_L2CAP_SDU_DATA_RX_EVENT                          30
#define ACI_GATT_SRV_ATTRIBUTE_MODIFIED_EVENT                31
#define ACI_GATT_PROC_TIMEOUT_EVENT                          32
#define ACI_ATT_EXCHANGE_MTU_RESP_EVENT                      33
#define ACI_ATT_CLT_FIND_INFO_RESP_EVENT                     34
#define ACI_ATT_CLT_FIND_BY_TYPE_VALUE_RESP_EVENT            35
#define ACI_ATT_CLT_READ_BY_TYPE_RESP_EVENT                  36
#define ACI_ATT_CLT_READ_RESP_EVENT                          37
#define ACI_ATT_CLT_READ_BLOB_RESP_EVENT                     38
#define ACI_ATT_CLT_READ_MULTIPLE_RESP_EVENT                 39
#define ACI_ATT_CLT_READ_BY_GROUP_TYPE_RESP_EVENT            40
#define ACI_ATT_CLT_PREPARE_WRITE_RESP_EVENT                 41
#define ACI_ATT_CLT_EXEC_WRITE_RESP_EVENT                    42
#define ACI_GATT_CLT_INDICATION_EVENT                        43
#define ACI_GATT_CLT_NOTIFICATION_EVENT                      44
#define ACI_GATT_CLT_PROC_COMPLETE_EVENT                     45
#define ACI_GATT_CLT_ERROR_RESP_EVENT                        46
#define ACI_GATT_CLT_DISC_READ_CHAR_BY_UUID_RESP_EVENT       47
#define ACI_GATT_TX_POOL_AVAILABLE_EVENT                     48
#define ACI_GATT_SRV_CONFIRMATION_EVENT                      49
#define ACI_GATT_SRV_READ_EVENT                              50
#define ACI_GATT_SRV_WRITE_EVENT                             51
#define ACI_ATT_SRV_PREPARE_WRITE_REQ_EVENT                  52
#define ACI_ATT_SRV_EXEC_WRITE_REQ_EVENT                     53
#define HCI_LE_CONNECTION_COMPLETE_EVENT                     54
#define HCI_LE_ADVERTISING_REPORT_EVENT                      55
#define HCI_LE_CONNECTION_UPDATE_COMPLETE_EVENT              56
#define HCI_LE_READ_REMOTE_USED_FEATURES_COMPLETE_EVENT      57
#define HCI_LE_LONG_TERM_KEY_REQUEST_EVENT                   58
#define HCI_LE_DATA_LENGTH_CHANGE_EVENT                      59
#define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT     60
#define HCI_LE_GENERATE_DHKEY_COMPLETE_EVENT                 61
#define HCI_LE_ENHANCED_CONNECTION_COMPLETE_EVENT            62
#define HCI_LE_DIRECT_ADVERTISING_REPORT_EVENT               63
#define HCI_LE_PHY_UPDATE_COMPLETE_EVENT                     64
#define HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT             65
#define HCI_LE_SCAN_TIMEOUT_EVENT                            66
#define HCI_LE_ADVERTISING_SET_TERMINATED_EVENT              67
#define HCI_LE_SCAN_REQUEST_RECEIVED_EVENT                   68
#define HCI_LE_CHANNEL_SELECTION_ALGORITHM_EVENT             69
#define REGISTER_CALLBACK(number, callback) ev_call_table[number] = (void *) callback
extern void * ev_call_table[70];
#endif /* _BLUENRG_LP_EV_IF_H_ */
