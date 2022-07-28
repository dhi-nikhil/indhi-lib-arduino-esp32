#include "BLEProvision.h"
#include "BLEController.h"
#include "BLEBluedroid.h"
#include "esp_log.h"
#include "esp_gatt_common_api.h"
#include "esp_gatts_api.h"

#if defined(ARDUINO_ARCH_ESP32)
#include "Arduino.h"
#include "esp32-hal-bt.h"
#endif

static const char * TAG = "BLEProvision";

esp_ble_adv_data_t BLEProvision::esp_ble_adv_data = { 0 };
esp_ble_adv_params_t BLEProvision::esp_ble_adv_params = { 0 };
ble_onboarding_profile_t BLEProvision::ble_onboarding_profile = { 0 };
service_config_t BLEProvision::wifi_scan_service = { 0 };
characteristics_config_t BLEProvision::wifi_scan_service_char_1 = { 0 };
characteristics_config_t BLEProvision::wifi_scan_service_char_2 = { 0 };
characteristics_config_t BLEProvision::wifi_scan_service_char_3 = { 0 };
service_config_t BLEProvision::wifi_config_service = { 0 };
characteristics_config_t BLEProvision::wifi_config_service_char_1 = { 0 };
characteristics_config_t BLEProvision::wifi_config_service_char_2 = { 0 };

uint8_t BLEProvision::adv_service_uuid128[32] = {
    0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0xAA,0x00,0x00,0x00,
    0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0xBB,0x00,0x00,0x00 };

BLEProvision::BLEProvision()
{

}

void BLEProvision::Init()
{
#if defined(ARDUINO_ARCH_ESP32)
    if (!btStarted() && !btStart())
    {
        ESP_LOGE(TAG, "btStart failed");
        return;
    }
#else
    BTController::BTControllerMemoryRelease(ESP_BT_MODE_CLASSIC_BT);
    BTController::Config();
    BTController::Init();
    BTController::Enable(ESP_BT_MODE_BLE);
#endif
    BLEBluedroid::Init();
    BLEBluedroid::Enable();
}

void BLEProvision::Deinit()
{
    BLEBluedroid::Disable();
    BLEBluedroid::Deinit();
#if defined(ARDUINO_ARCH_ESP32)
    if (!btStop())
    {
        ESP_LOGE(TAG, "btStop failed");
        return;
    }
#else
    BTController::Disable();
    BTController::Deinit();
#endif
}

void BLEProvision::SetName(std::string name)
{
    mName = name;
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_set_device_name(mName.c_str()));
}


void BLEProvision::BLEGAPConfigure()
{
    esp_ble_adv_data.set_scan_rsp = false;
    esp_ble_adv_data.include_name = true;
    esp_ble_adv_data.min_interval = 512; //slave connection min interval, Time = min_interval * 1.25 msec
    esp_ble_adv_data.max_interval = 1024; //slave connection max interval, Time = max_interval * 1.25 msec
    esp_ble_adv_data.service_data_len = 0;
    esp_ble_adv_data.p_service_data = NULL;
    esp_ble_adv_data.service_uuid_len = sizeof(adv_service_uuid128);
    esp_ble_adv_data.p_service_uuid = &adv_service_uuid128[0];
    esp_ble_adv_data.flag = ( ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT );

    esp_ble_adv_params.adv_int_min = 512;
    esp_ble_adv_params.adv_int_max = 1024;
    esp_ble_adv_params.adv_type = ADV_TYPE_IND;
    esp_ble_adv_params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
    esp_ble_adv_params.channel_map = ADV_CHNL_ALL;
    esp_ble_adv_params.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_config_adv_data(&esp_ble_adv_data));

}

void BLEProvision::StartAdvertising()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_start_advertising(&esp_ble_adv_params));
}

void BLEProvision::StopAdvertising()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gap_stop_advertising());
}

void BLEProvision::ConfigureProvisionGATT()
{
    wifi_scan_service_char_1.char_handle = 0;
    wifi_scan_service_char_1.char_uuid.len = ESP_UUID_LEN_16;
    wifi_scan_service_char_1.char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_A;
    wifi_scan_service_char_1.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
    wifi_scan_service_char_1.property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;
    wifi_scan_service_char_1.val.attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX;

    wifi_scan_service_char_2.char_handle = 0;
    wifi_scan_service_char_2.char_uuid.len = ESP_UUID_LEN_16;
    wifi_scan_service_char_2.char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_B;
    wifi_scan_service_char_2.perm = ESP_GATT_PERM_READ;
    wifi_scan_service_char_2.property = ESP_GATT_CHAR_PROP_BIT_READ;

    wifi_scan_service_char_3.char_handle = 0;
    wifi_scan_service_char_3.char_uuid.len = ESP_UUID_LEN_16;
    wifi_scan_service_char_3.char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_C;
    wifi_scan_service_char_3.perm = ESP_GATT_PERM_WRITE;
    wifi_scan_service_char_3.property = ESP_GATT_CHAR_PROP_BIT_WRITE;

    wifi_config_service_char_1.char_handle = 0;
    wifi_config_service_char_1.char_uuid.len = ESP_UUID_LEN_16;
    wifi_config_service_char_1.char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_D;
    wifi_config_service_char_1.perm = ESP_GATT_PERM_READ;
    wifi_config_service_char_1.property = ESP_GATT_CHAR_PROP_BIT_READ;

    wifi_config_service_char_2.char_handle = 0;
    wifi_config_service_char_2.char_uuid.len = ESP_UUID_LEN_16;
    wifi_config_service_char_2.char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_E;
    wifi_config_service_char_2.perm = ESP_GATT_PERM_WRITE;
    wifi_config_service_char_2.property = ESP_GATT_CHAR_PROP_BIT_WRITE;

    wifi_scan_service.service_handle = 0;
    wifi_scan_service.service_num_handle = 7;
    wifi_scan_service.service_id.is_primary = true;
    wifi_scan_service.service_id.id.inst_id = 0x00;
    wifi_scan_service.service_id.id.uuid.len = ESP_UUID_LEN_16;
    wifi_scan_service.service_id.id.uuid.uuid.uuid16 = WIFI_SCAN_SERVICE_UUID;
    wifi_scan_service.charas_num = 3;
    wifi_scan_service.charas[0] = &wifi_scan_service_char_1;
    wifi_scan_service.charas[1] = &wifi_scan_service_char_2;
    wifi_scan_service.charas[2] = &wifi_scan_service_char_3;

    wifi_config_service.service_handle = 0;
    wifi_config_service.service_num_handle = 7;
    wifi_config_service.service_id.is_primary = true;
    wifi_config_service.service_id.id.inst_id = 0x00;
    wifi_config_service.service_id.id.uuid.len = ESP_UUID_LEN_16;
    wifi_config_service.service_id.id.uuid.uuid.uuid16 = WIFI_CONF_SERVICE_UUID;
    wifi_config_service.charas_num = 2;
    wifi_config_service.charas[0] = &wifi_config_service_char_1;
    wifi_config_service.charas[1] = &wifi_config_service_char_2;

    ble_onboarding_profile.gatts_cb = GATTCallback;
    ble_onboarding_profile.gatts_if = ESP_GATT_IF_NONE;
    ble_onboarding_profile.app_id = 0;
    ble_onboarding_profile.conn_id = 0;
    ble_onboarding_profile.wifi_scan = &wifi_scan_service;
    ble_onboarding_profile.wifi_config = &wifi_config_service;
}

void BLEProvision::Start()
{
    if (GATTCallback != nullptr)
    {
        esp_ble_gatts_register_callback(GATTCallback);
        esp_ble_gatts_app_register(0x55);
        esp_ble_gatt_set_local_mtu(517);
    }
    if (GAPCallback != nullptr)
    {
        esp_ble_gap_register_callback(GAPCallback);
    }
}

void BLEProvision::Stop()
{
    StopAdvertising();
}
