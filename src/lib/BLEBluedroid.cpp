#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "BLEBluedroid.h"
#include "string"
#include "string.h"
#include "esp_log.h"
#include "Utils.h"

static const char * TAG = "BLEBluedroid";

void BLEBluedroid::Init(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bluedroid_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_bluedroid_init: rc=%d %s", err, Utils::Utils::ErrorToString(err));
        return;
    }
}

void BLEBluedroid::Deinit(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bluedroid_deinit();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_bluedroid_deinit: rc=%d %s", err, Utils::Utils::ErrorToString(err));
        return;
    }
}

void BLEBluedroid::Enable(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bluedroid_enable();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_bluedroid_enable: rc=%d %s", err, Utils::Utils::ErrorToString(err));
        return;
    }
}

void BLEBluedroid::Disable(void)
{
    esp_err_t err = ESP_OK;
    err = esp_bluedroid_disable();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_bluedroid_disable: rc=%d %s", err, Utils::Utils::ErrorToString(err));
        return;
    }
}

esp_bluedroid_status_t BLEBluedroid::GetStatus(void)
{
    return esp_bluedroid_get_status();
}
