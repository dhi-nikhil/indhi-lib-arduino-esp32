#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "BLEController.h"
#include "string"
#include "string.h"
#include "esp_log.h"
#include "Utils.h"

static const char * TAG = "BTController";

esp_bt_controller_config_t BTController::mBTControllerConfig = {};

void BTController::Config(void)
{
    mBTControllerConfig.controller_task_stack_size = ESP_TASK_BT_CONTROLLER_STACK;
    mBTControllerConfig.controller_task_prio = ESP_TASK_BT_CONTROLLER_PRIO;
    mBTControllerConfig.hci_uart_no = BT_HCI_UART_NO_DEFAULT;
    mBTControllerConfig.hci_uart_baudrate = BT_HCI_UART_BAUDRATE_DEFAULT;
    mBTControllerConfig.scan_duplicate_mode = SCAN_DUPLICATE_MODE;
    mBTControllerConfig.scan_duplicate_type = SCAN_DUPLICATE_TYPE_VALUE;
    mBTControllerConfig.normal_adv_size = NORMAL_SCAN_DUPLICATE_CACHE_SIZE;
    mBTControllerConfig.mesh_adv_size = MESH_DUPLICATE_SCAN_CACHE_SIZE;
    mBTControllerConfig.send_adv_reserved_size = SCAN_SEND_ADV_RESERVED_SIZE;
    mBTControllerConfig.controller_debug_flag = CONTROLLER_ADV_LOST_DEBUG_BIT;
    mBTControllerConfig.mode = BTDM_CONTROLLER_MODE_EFF;
    mBTControllerConfig.ble_max_conn = CONFIG_BTDM_CTRL_BLE_MAX_CONN_EFF;
    mBTControllerConfig.bt_max_acl_conn = CONFIG_BTDM_CTRL_BR_EDR_MAX_ACL_CONN_EFF;
    mBTControllerConfig.bt_sco_datapath = CONFIG_BTDM_CTRL_BR_EDR_SCO_DATA_PATH_EFF;
    mBTControllerConfig.auto_latency = BTDM_CTRL_AUTO_LATENCY_EFF;
    mBTControllerConfig.bt_legacy_auth_vs_evt = BTDM_CTRL_LEGACY_AUTH_VENDOR_EVT_EFF;
    mBTControllerConfig.bt_max_sync_conn = CONFIG_BTDM_CTRL_BR_EDR_MAX_SYNC_CONN_EFF;
    mBTControllerConfig.ble_sca = CONFIG_BTDM_BLE_SLEEP_CLOCK_ACCURACY_INDEX_EFF;
    mBTControllerConfig.pcm_role = CONFIG_BTDM_CTRL_PCM_ROLE_EFF;
    mBTControllerConfig.pcm_polar = CONFIG_BTDM_CTRL_PCM_POLAR_EFF;
    mBTControllerConfig.hli = BTDM_CTRL_HLI;
    mBTControllerConfig.magic = ESP_BT_CONTROLLER_CONFIG_MAGIC_VAL;
}

void BTController::Init(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_controller_init(&mBTControllerConfig);
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_controller_init: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

void BTController::Deinit(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_controller_deinit();
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_controller_deinit: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

void BTController::Enable(esp_bt_mode_t bluetoothMode)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_controller_enable(bluetoothMode);
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_controller_enable: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

void BTController::Disable(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_controller_disable();
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_controller_disable: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

esp_bt_controller_status_t BTController::GetStatus(void)
{
    return esp_bt_controller_get_status();
}

void BTController::BTControllerMemoryRelease(esp_bt_mode_t bluetoothMode)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_controller_mem_release(bluetoothMode);
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_controller_mem_release: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

void BTController::BTMemoryRelease(esp_bt_mode_t bluetoothMode)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_mem_release(bluetoothMode);
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_mem_release: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

void BTController::SleepModeEnable(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_sleep_disable();
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_sleep_disable: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}

void BTController::SleepModeDisable(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bt_sleep_enable();
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        ESP_LOGE(TAG, "esp_bt_sleep_enable: %d ( %s )", err, esp_err_to_name(err));
        return;
    }
}
