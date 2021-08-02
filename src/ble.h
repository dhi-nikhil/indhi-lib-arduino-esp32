#ifndef BLE_H_
#define BLE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "log_handler.h"
#include "debug_log.h"
#include "helper.h"
#include "error_code_manager.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include "sdkconfig.h"

typedef struct
{
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    esp_attr_value_t val;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
} characteristics_config_t;

typedef struct
{
    uint16_t service_handle;
    uint16_t service_num_handle;
    esp_gatt_srvc_id_t service_id;
    int charas_num;
    characteristics_config_t *charas[3];
} service_config_t;

typedef struct
{
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    service_config_t *wifi_scan;
    service_config_t *wifi_config;
} ble_onboarding_profile_t;

ErrorCode_t ble_provisioning_init(void);
ErrorCode_t ble_provisioning_deinit(void);
ErrorCode_t ble_gap_init(void);
ErrorCode_t ble_gap_config(void);
ErrorCode_t ble_gap_start(void);
ErrorCode_t ble_gap_stop(void);
ErrorCode_t ble_gap_deinit(void);
ErrorCode_t ble_gatt_init(void);
ErrorCode_t ble_gatt_config(void);
ErrorCode_t ble_gatt_start(void);
ErrorCode_t ble_gatt_stop(void);
ErrorCode_t ble_gatt_deinit(void);

ErrorCode_t ble_gap_config_adv_data(void);
ErrorCode_t ble_gap_config_rsp_data(void);
ErrorCode_t ble_gap_start_advertising(void);
ErrorCode_t ble_gap_stop_advertising(void);

ErrorCode_t ble_gap_set_device_name(void);
ErrorCode_t ble_gatt_app_prov_init(void);
ErrorCode_t ble_gatt_app_prov_deinit(void);
#endif