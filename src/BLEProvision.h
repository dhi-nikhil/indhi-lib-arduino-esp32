#ifndef BLE_PROVISION_H_
#define BLE_PROVISION_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_gatts_api.h"
#include "string.h"
#include "string"
#include "esp_gap_ble_api.h"

/**
 * @brief GATT characteristics structure used to store and manage the characteristics
 *
 */
typedef struct
{
    uint16_t char_handle;   /* Used to store the characteristics handle which is used to manage the characteristics */
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
    characteristics_config_t * charas[3];
} service_config_t;

typedef struct
{
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    service_config_t * wifi_scan;
    service_config_t * wifi_config;
} ble_onboarding_profile_t;

class BLEProvision
{
protected:
    std::string mName;
    static esp_ble_adv_data_t esp_ble_adv_data;
    static esp_ble_adv_params_t esp_ble_adv_params;

    static service_config_t wifi_scan_service;
    static characteristics_config_t wifi_scan_service_char_1;
    static characteristics_config_t wifi_scan_service_char_2;
    static characteristics_config_t wifi_scan_service_char_3;
    static service_config_t wifi_config_service;
    static characteristics_config_t wifi_config_service_char_1;
    static characteristics_config_t wifi_config_service_char_2;

    static ble_onboarding_profile_t ble_onboarding_profile;

    static uint8_t adv_service_uuid128[32];

    // Wi-Fi Scanner State Machine States
    static const int WIFI_SCANNER_STATE_IDLE = 0;
    static const int WIFI_SCANNER_STATE_SCAN = 1;
    static const int WIFI_SCANNER_STATE_SCANNING = 2;
    static const int WIFI_SCANNER_STATE_SCANNED = 3;
    static const int WIFI_SCANNER_STATE_ERROR = 4;

    // Wi-Fi Config State Machine States
    static const int WIFI_CONFIG_STATE_IDLE = 0;
    static const int WIFI_CONFIG_STATE_SAVE = 1;
    static const int WIFI_CONFIG_STATE_SAVING = 2;
    static const int WIFI_CONFIG_STATE_SAVED = 3;
    static const int WIFI_CONFIG_STATE_JOIN = 4;
    static const int WIFI_CONFIG_STATE_JOINING = 5;
    static const int WIFI_CONFIG_STATE_JOINED = 6;
    static const int WIFI_CONFIG_STATE_ERROR = 7;

    static const int GATTS_DEMO_CHAR_VAL_LEN_MAX = 500;
    static const int WIFI_SERVICE_HANDLES_NO = 7;

    static const int WIFI_SCAN_SERVICE_UUID = 0x00BB;
    static const int GATTS_CHAR_UUID_A = 0xBB01;
    static const int GATTS_CHAR_UUID_B = 0xBB02;
    static const int GATTS_CHAR_UUID_C = 0xBB03;

    static const int WIFI_CONF_SERVICE_UUID = 0x00AA;
    static const int GATTS_CHAR_UUID_D = 0xAA01;
    static const int GATTS_CHAR_UUID_E = 0xAA02;

    static const int PREPARE_BUF_MAX_SIZE = 1024;

public:
    BLEProvision();
    void Init();
    void Deinit();
    void SetName(std::string name);
    static void BLEGAPConfigure();
    static void StartAdvertising();
    static void StopAdvertising();
    void Start();
    void Stop();
    void ConfigureProvisionGATT();

    esp_gap_ble_cb_t GAPCallback;
    esp_gatts_cb_t GATTCallback;

    void SetGAPCallback(esp_gap_ble_cb_t callback)
    {
        GAPCallback = callback;
    }
    void SetGATTCallback(esp_gatts_cb_t callback)
    {
        GATTCallback = callback;
    }

};

#endif
