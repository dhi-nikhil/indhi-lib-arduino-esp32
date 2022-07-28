#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "WiFi.h"
#include "WiFiStation.h"
#include "esp_log.h"

// #include <stdint.h>
// #include <stdbool.h>
// #include <string.h>
// #include <string>
// #include <stdio.h>
// #include <stdlib.h>
// #include <inttypes.h>
// #include <string.h>
// #include <esp_err.h>
// #include <esp_wifi.h>
// #include <esp_event_loop.h>
// #include <lwip/ip_addr.h>
// #include "lwip/err.h"
// #include "lwip/dns.h"

static const char * TAG = "WiFiStation";

static esp_netif_t * mSTANetIF = NULL;

bool WiFiStation::mAutoReconnect = true;

static bool sta_config_equal(const wifi_config_t & lhs, const wifi_config_t & rhs);

static bool sta_config_equal(const wifi_config_t & lhs, const wifi_config_t & rhs)
{
    if (memcmp(&lhs, &rhs, sizeof(wifi_config_t)) != 0)
    {
        return false;
    }
    return true;
}

WiFiStation::WiFiStation() : WiFiSTAEventGroup()
{
    ESP_LOGI(TAG, "%s", __func__);
    Create();
}

WiFiStation::~WiFiStation()
{
    ESP_LOGI(TAG, "%s", __func__);
    Delete();
}

void WiFiStation::Create()
{
    mSTANetIF = esp_netif_create_default_wifi_sta();
}

void WiFiStation::Delete()
{
    esp_netif_destroy_default_wifi(mSTANetIF);
    mSTANetIF = nullptr;
}


/* this should not be here or enableSTA should have called above */
esp_err_t WiFiStation::STAConfig(std::string ssid, std::string passphrase, int32_t channel, const uint8_t * bssid, bool connect)
{
    ESP_LOGI(TAG, "STACONFIG");
    if (WiFi::EnableSTA(true) != ESP_OK)
    {
        ESP_LOGE(TAG, "STA enable failed!");
        return ESP_FAIL;
    }

    if (ssid.empty() || ssid.length() > 32)
    {
        ESP_LOGE(TAG, "SSID too long or missing!");
        return ESP_FAIL;
    }

    if (passphrase.length() > 64)
    {
        ESP_LOGE(TAG, "passphrase too long!");
        return ESP_FAIL;
    }

    wifi_config_t conf;
    memset(&conf, 0, sizeof(wifi_config_t));
    strlcpy(reinterpret_cast<char *>( conf.sta.ssid ), ssid.c_str(), sizeof(conf.sta.ssid));
    conf.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;       //force full scan to be able to choose the nearest / strongest AP

    if (!passphrase.empty())
    {
        if (passphrase.length() == 64)
        {
            memcpy(reinterpret_cast<char *>( conf.sta.password ), passphrase.c_str(), 64);
        }
        else
        {
            strcpy(reinterpret_cast<char *>( conf.sta.password ), passphrase.c_str());
        }
    }

    if (bssid)
    {
        conf.sta.bssid_set = 1;
        memcpy((void *) &conf.sta.bssid[0], (void *) bssid, 6);
    }

    if (channel > 0 && channel <= 13)
    {
        conf.sta.channel = channel;
    }

    WiFi::Start();

    wifi_config_t current_conf;
    esp_wifi_get_config(WIFI_IF_STA, &current_conf);
    // if (!sta_config_equal(current_conf, conf))
    // {
    //     if (esp_wifi_disconnect() != ESP_OK)
    //     {
    //         ESP_LOGE(TAG, "disconnect failed!");
    //         return ESP_FAIL;
    //     }

    //     esp_wifi_set_config(WIFI_IF_STA, &conf);
    // }
    // else
    // {
    esp_wifi_set_config(WIFI_IF_STA, &conf);
    // }


    esp_err_t err;
    if (connect)
    {
        err = esp_wifi_connect();
        if (err)
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
            return ESP_FAIL;
        }
    }

    return ESP_OK;
}

esp_err_t WiFiStation::Connect()
{
    return ::esp_wifi_connect();
}

esp_err_t WiFiStation::Reconnect()
{
    if (WiFi::GetMode() & WIFI_MODE_STA)
    {
        if (esp_wifi_disconnect() == ESP_OK)
        {
            return esp_wifi_connect();
        }
    }
    return ESP_OK;
}

esp_err_t WiFiStation::Disconnect(bool wifioff, bool eraseap)
{
    wifi_config_t conf;

    if (WiFi::GetMode() & WIFI_MODE_STA)
    {
        if (eraseap)
        {
            memset(&conf, 0, sizeof(wifi_config_t));
            if (esp_wifi_set_config(WIFI_IF_STA, &conf))
            {
                ESP_LOGE(TAG, "clear config failed!");
            }
        }
        if (esp_wifi_disconnect())
        {
            ESP_LOGE(TAG, "disconnect failed!");
            return ESP_FAIL;
        }
        if (wifioff)
        {
            return WiFi::EnableSTA(false);
        }
        return ESP_OK;
    }
    return ESP_FAIL;
}

bool WiFiStation::SetAutoReconnect(bool autoReconnect)
{
    mAutoReconnect = autoReconnect;
    return true;
}

bool WiFiStation::GetAutoReconnect()
{
    return mAutoReconnect;
}

uint8_t * WiFiStation::MACAddress(uint8_t * mac)
{
    if (WiFi::GetMode() != WIFI_MODE_NULL)
    {
        esp_wifi_get_mac(WIFI_IF_STA, mac);
    }
    else
    {
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
    }
    return mac;
}

std::string WiFiStation::MACAddress(void)
{
    uint8_t mac[6];
    char macStr[18] = { 0 };
    if (WiFi::GetMode() == WIFI_MODE_NULL)
    {
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
    }
    else
    {
        esp_wifi_get_mac(WIFI_IF_STA, mac);
    }
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(macStr);
}

std::string WiFiStation::SSID() const
{
    if (WiFi::GetMode() == WIFI_MODE_NULL)
    {
        return std::string();
    }
    wifi_ap_record_t info;
    if (!esp_wifi_sta_get_ap_info(&info))
    {
        return std::string(reinterpret_cast<char *>( info.ssid ));
    }
    return std::string();
}

std::string WiFiStation::PSK() const
{
    if (WiFi::GetMode() == WIFI_MODE_NULL)
    {
        return std::string();
    }
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return std::string(reinterpret_cast<char *>( conf.sta.password ));
}

int8_t WiFiStation::RSSI(void)
{
    if (WiFi::GetMode() == WIFI_MODE_NULL)
    {
        return 0;
    }
    wifi_ap_record_t info;
    if (!esp_wifi_sta_get_ap_info(&info))
    {
        return info.rssi;
    }
    return 0;
}
