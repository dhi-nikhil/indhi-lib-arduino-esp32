#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "WiFiStationScan.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "string"
#include "string.h"

static const char * TAG = "WiFiStationScan";

WiFiStationScan::WiFiScanStatus WiFiStationScan::Scan(bool block)
{
    WiFiStationScan::WiFiScanStatus wifiScanStatus;
    esp_err_t err;
    wifi_scan_config_t wifi_scan_config = {};
    err = esp_wifi_scan_start(&wifi_scan_config, block);

    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    if (err)
    {
        wifiScanStatus = WiFiScanStatus::SCAN_ERROR;
        WiFiScanEventGroup::EventGroupSetBits(WiFiScanEventGroup::EG_SCAN_ERROR);
    }
    else
    {
        if (block)
        {
            wifiScanStatus = WiFiScanStatus::SCAN_COMPLETED;
            WiFiScanEventGroup::EventGroupSetBits(WiFiScanEventGroup::EG_SCAN_COMPLETED);
        }
        else
        {
            wifiScanStatus = WiFiScanStatus::SCAN_INPROCESS;
            WiFiScanEventGroup::EventGroupSetBits(WiFiScanEventGroup::EG_SCAN_INPROCESS);
        }
    }
    ESP_LOGI(TAG, "WiFi status %d", wifiScanStatus);
    return wifiScanStatus;
}


WiFiStationScan::WiFiScanStatus WiFiStationScan::IsScanComplete()
{
    WiFiStationScan::WiFiScanStatus wifiScanStatus;
    EventBits_t eventBits;

    eventBits = WiFiScanEventGroup::EventGroupGetBits();

    if (eventBits & EG_SCAN_COMPLETED)
    {
        wifiScanStatus = SCAN_COMPLETED;
    }
    else if (eventBits & EG_SCAN_INPROCESS)
    {
        wifiScanStatus = SCAN_INPROCESS;
    }
    else if (eventBits & EG_SCAN_ERROR)
    {
        wifiScanStatus = SCAN_ERROR;
    }
    else
    {
        wifiScanStatus = SCAN_IDLE;
    }
    ESP_LOGI(TAG, "WiFi status %d", wifiScanStatus);
    return wifiScanStatus;
}

uint16_t WiFiStationScan::GetScanEntries(uint16_t entries)
{
    WiFiStationScan::WiFiScanStatus wifiScanStatus;
    wifiScanStatus = IsScanComplete();
    if (wifiScanStatus == SCAN_IDLE || wifiScanStatus == SCAN_ERROR || wifiScanStatus == SCAN_INPROCESS)
    {
        ESP_LOGE(TAG, "WiFi scan is not called or error occured while scanning");
        return 0;   //  Error
    }

    esp_err_t err;
    mWiFiAPRecord = (wifi_ap_record_t *) calloc(entries, sizeof(wifi_ap_record_t));
    err = esp_wifi_scan_get_ap_records(&entries, mWiFiAPRecord);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "error while scanning : %s (%d)", esp_err_to_name(err), err);
        return 0;   //  Error
    }

    return entries;
}

std::string WiFiStationScan::SSID(uint16_t index)
{
    return std::string((char *) ( mWiFiAPRecord + index )->ssid);
}

int8_t WiFiStationScan::RSSI(uint16_t index)
{
    return ( ( mWiFiAPRecord + index )->rssi );
}

std::string WiFiStationScan::EncryptionType(uint16_t index)
{
    std::string authMode;
    wifi_auth_mode_t wifiAuthMode;

    wifiAuthMode = (wifi_auth_mode_t) ( mWiFiAPRecord + index )->authmode;

    switch (wifiAuthMode)
    {
        case WIFI_AUTH_OPEN:
        {
            authMode = "WIFI_AUTH_OPEN";         /**< authenticate mode : open */
            break;
        }
        case WIFI_AUTH_WEP:
        {
            authMode = "WIFI_AUTH_WEP";              /**< authenticate mode : WEP */
            break;
        }
        case WIFI_AUTH_WPA_PSK:
        {
            authMode = "WIFI_AUTH_WPA_PSK";          /**< authenticate mode : WPA_PSK */
            break;
        }
        case WIFI_AUTH_WPA2_PSK:
        {
            authMode = "WIFI_AUTH_WPA2_PSK";         /**< authenticate mode : WPA2_PSK */
            break;
        }
        case WIFI_AUTH_WPA_WPA2_PSK:
        {
            authMode = "WIFI_AUTH_WPA_WPA2_PSK";     /**< authenticate mode : WPA_WPA2_PSK */
            break;
        }
        case WIFI_AUTH_WPA2_ENTERPRISE:
        {
            authMode = "WIFI_AUTH_WPA2_ENTERPRISE";  /**< authenticate mode : WPA2_ENTERPRISE */
            break;
        }
        case WIFI_AUTH_WPA3_PSK:
        {
            authMode = "WIFI_AUTH_WPA3_PSK";         /**< authenticate mode : WPA3_PSK */
            break;
        }
        case WIFI_AUTH_WPA2_WPA3_PSK:
        {
            authMode = "WIFI_AUTH_WPA2_WPA3_PSK";    /**< authenticate mode : WPA2_WPA3_PSK */
            break;
        }
        case WIFI_AUTH_WAPI_PSK:
        {
            authMode = "WIFI_AUTH_WAPI_PSK";         /**< authenticate mode : WAPI_PSK */
            break;
        }
        default:
        {
            authMode = "INVALID";         /**< authenticate mode : WAPI_PSK */
            break;
        }
    }
    return authMode;
}

