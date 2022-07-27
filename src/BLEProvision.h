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
    esp_bt_uuid_t char_uuid;    /* Used to store the characteritics uuid */
    esp_gatt_perm_t perm;   /* used to store the permissions related to characteristics */
    esp_gatt_char_prop_t property;  /* used to store the property of the characterisitcs */
    esp_attr_value_t val;   /* Used to store the read and write data */
    uint16_t descr_handle;  /* used to store descriptor handle */
    esp_bt_uuid_t descr_uuid;   /* used to store descriptor characteristics uuid */
} characteristics_config_t;

/**
 * @brief GATT service structure used to store and manage the services
 *
 */
typedef struct
{
    uint16_t service_handle;    /* Used to store the service handle which is used to manage the services */
    uint16_t service_num_handle;    /* Used to store the maximum number of handles alloweded */
    esp_gatt_srvc_id_t service_id;  /* Used to store the service id */
    int charas_num; /* Used to store the maximum characteristics supported in the service */
    characteristics_config_t * charas[3];   /* Space for the characteristics */
} service_config_t;

/**
 * @brief GATT profile structure used to store the all the data related to services and everything
 *
 */
typedef struct
{
    esp_gatts_cb_t gatts_cb;    /* GATT event callback */
    uint16_t gatts_if;  /* GATT if ID store */
    uint16_t app_id;    /* Used to store app id */
    uint16_t conn_id;   /* Used to store the connection id */
    service_config_t * wifi_scan;   /* Used to store the wifi scan config */
    service_config_t * wifi_config; /* Used to store the wifi config config */
} ble_onboarding_profile_t;

/**
 * @brief Used to store the BLE Provisioning data
 *
 */
class BLEProvision
{
protected:
    /**
     * @brief Name of the ble device
     *
     */
    std::string mName;
    /**
     * @brief Store the advertisement data
     *
     */
    static esp_ble_adv_data_t esp_ble_adv_data;
    /**
     * @brief Used to store the advertising parameter
     *
     */
    static esp_ble_adv_params_t esp_ble_adv_params;

    /**
     * @brief Used to store the wifi scan service configuration
     *
     */
    static service_config_t wifi_scan_service;
    /**
     * @brief Used to store the wifi scan service 1 which handles the command and status
     *
     */
    static characteristics_config_t wifi_scan_service_char_1;
    /**
     * @brief Used to store the wifi scan service 2 which handles the response of the scan result
     *
     */
    static characteristics_config_t wifi_scan_service_char_2;
    /**
     * @brief used to store the wifi scan service 3 which handles the which ssid and password
     *
     */
    static characteristics_config_t wifi_scan_service_char_3;
    /**
     * @brief Used to store the wifi config service configuration
     *
     */
    static service_config_t wifi_config_service;
    /**
     * @brief Used to store the wifi config service 1 which handles the status of the configuration
     *
     */
    static characteristics_config_t wifi_config_service_char_1;
    /**
     * @brief Used to store the wifi config service 2 which handles the additional data related to server
     *
     */
    static characteristics_config_t wifi_config_service_char_2;

    /**
     * @brief Used to store the entire  GATT profile
     *
     */
    static ble_onboarding_profile_t ble_onboarding_profile;

    /**
     * @brief Used to store the servce uuids
     *
     */
    static uint8_t adv_service_uuid128[32];

    // Wi-Fi Scanner State Machine States
    static const int WIFI_SCANNER_STATE_IDLE = 0;   /* Indicate the idle state of wifi scanner and config */
    static const int WIFI_SCANNER_STATE_SCAN = 1;   /* Indicate the scan command */
    static const int WIFI_SCANNER_STATE_SCANNING = 2;   /* Indicate the wifi scanning state */
    static const int WIFI_SCANNER_STATE_SCANNED = 3;    /* Indicate the wifi scanned state */
    static const int WIFI_SCANNER_STATE_ERROR = 4;  /* indicate the error while processing scanning */

    // Wi-Fi Config State Machine States
    static const int WIFI_CONFIG_STATE_IDLE = 0;    /* Indicate the idle state of wifi config */
    static const int WIFI_CONFIG_STATE_SAVE = 1;    /* Not used */
    static const int WIFI_CONFIG_STATE_SAVING = 2;/* Not used */
    static const int WIFI_CONFIG_STATE_SAVED = 3;/* Not used */
    static const int WIFI_CONFIG_STATE_JOIN = 4;/* Not used */
    static const int WIFI_CONFIG_STATE_JOINING = 5;/* Not used */
    static const int WIFI_CONFIG_STATE_JOINED = 6;/* Not used */
    static const int WIFI_CONFIG_STATE_ERROR = 7;/* Not used */

    static const int GATTS_DEMO_CHAR_VAL_LEN_MAX = 500; /* Maximum value of characteristics */
    static const int WIFI_SERVICE_HANDLES_NO = 7;   /* number of handles for the service */

    static const int WIFI_SCAN_SERVICE_UUID = 0x00BB;   /* Short uuid for wifi scan */
    static const int GATTS_CHAR_UUID_A = 0xBB01;    /* short uuid for characteristics */
    static const int GATTS_CHAR_UUID_B = 0xBB02;    /* short uuid for characteristics */
    static const int GATTS_CHAR_UUID_C = 0xBB03;    /* short uuid for characteristics */

    static const int WIFI_CONF_SERVICE_UUID = 0x00AA;   /* Short uuid for wifi config */
    static const int GATTS_CHAR_UUID_D = 0xAA01;    /* short uuid for characteristics */
    static const int GATTS_CHAR_UUID_E = 0xAA02;    /* short uuid for characteristics */

    static const int PREPARE_BUF_MAX_SIZE = 1024;

public:
    /**
     * @brief Construct a new BLEProvision object
     *
     */
    BLEProvision();
    /**
     * @brief Initialize the proisioning using BLE
     *
     */
    void Init();
    /**
     * @brief Deinitialize the provisioning using BLE
     *
     */
    void Deinit();
    /**
     * @brief Set the Name of ble
     *
     * @param name
     */
    void SetName(std::string name);
    /**
     * @brief Configure the GAP
     *
     */
    static void BLEGAPConfigure();
    /**
     * @brief Start the advertising
     *
     */
    static void StartAdvertising();
    /**
     * @brief Stop the advertising
     *
     */
    static void StopAdvertising();
    /**
     * @brief Start the Provisioning procees using BLE
     *
     */
    void Start();
    /**
     * @brief Stop the provisioning proceess using BLE
     *
     */
    void Stop();
    /**
     * @brief Configure the GATT services
     *
     */
    void ConfigureProvisionGATT();
    /**
     * @brief Store the GAP callback
     *
     */
    esp_gap_ble_cb_t GAPCallback;
    /**
     * @brief Store the GATT callback
     *
     */
    esp_gatts_cb_t GATTCallback;

    /**
     * @brief Store the GAP callback
     *
     * @param callback GAP callbaack
     */
    void SetGAPCallback(esp_gap_ble_cb_t callback)
    {
        GAPCallback = callback;
    }
    /**
     * @brief Store the GATT callback
     *
     * @param callback
     */
    void SetGATTCallback(esp_gatts_cb_t callback)
    {
        GATTCallback = callback;
    }

};

#endif
