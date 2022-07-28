#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "WiFi.h"
#include "WiFiAccessPoint.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "esp_log.h"
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <lwip/ip_addr.h>
#include "dhcpserver/dhcpserver_options.h"

static const char * TAG = "WiFiAccessPoint";

esp_netif_t * mAPNetIF = NULL;

static bool softap_config_equal(const wifi_config_t & lhs, const wifi_config_t & rhs);

static bool softap_config_equal(const wifi_config_t & lhs, const wifi_config_t & rhs)
{
    if (strcmp(reinterpret_cast<const char *>( lhs.ap.ssid ), reinterpret_cast<const char *>( rhs.ap.ssid )) != 0)
    {
        return false;
    }
    if (strcmp(reinterpret_cast<const char *>( lhs.ap.password ), reinterpret_cast<const char *>( rhs.ap.password )) != 0)
    {
        return false;
    }
    if (lhs.ap.channel != rhs.ap.channel)
    {
        return false;
    }
    if (lhs.ap.ssid_hidden != rhs.ap.ssid_hidden)
    {
        return false;
    }
    if (lhs.ap.max_connection != rhs.ap.max_connection)
    {
        return false;
    }
    return true;
}

WiFiAccessPoint::WiFiAccessPoint() : WiFiAPEventGroup()
{
    Create();
}

WiFiAccessPoint::~WiFiAccessPoint()
{
    Delete();
}

void WiFiAccessPoint::Create()
{
    mAPNetIF = esp_netif_create_default_wifi_ap();
    if (mAPNetIF == nullptr)
    {
        ESP_LOGE(TAG, "ERROR CREATING DEFAULT AP");
    }
}

void WiFiAccessPoint::Delete()
{
    esp_netif_destroy_default_wifi(mAPNetIF);
    mAPNetIF = nullptr;
}

/* modify it accoring to new logic */
/* change this to string */
bool WiFiAccessPoint::SoftAPConfig(std::string ssid, std::string passphrase, int channel, int ssid_hidden, int max_connection)
{
    if (WiFi::EnableAP(true) != ESP_OK)
    {
        ESP_LOGE(TAG, "enable AP first!");
        return false;
    }

    if (ssid.empty() || ssid.length() == 0)
    {
        ESP_LOGE(TAG, "SSI too long or missing!");
        return false;
    }

    if (passphrase.length() > 64)
    {
        ESP_LOGE(TAG, "passphrase too long!");
        return false;
    }

    WiFi::Start();

    wifi_config_t conf;
    memset(&conf, 0, sizeof(wifi_config_t));
    strlcpy(reinterpret_cast<char *>( conf.ap.ssid ), ssid.c_str(), 33);
    conf.ap.channel = channel;
    conf.ap.ssid_len = strlen(reinterpret_cast<char *>( conf.ap.ssid ));
    conf.ap.ssid_hidden = ssid_hidden;
    conf.ap.max_connection = max_connection;
    conf.ap.beacon_interval = 100;

    if (passphrase.empty() || passphrase.length() == 0)
    {
        conf.ap.authmode = WIFI_AUTH_OPEN;
        *conf.ap.password = 0;
    }
    else
    {
        conf.ap.authmode = WIFI_AUTH_WPA2_PSK;
        strlcpy(reinterpret_cast<char *>( conf.ap.password ), passphrase.c_str(), sizeof(conf.ap.password));
    }

    // ESP_LOG_BUFFER_CHAR_LEVEL(TAG, conf.ap.ssid, 32, ESP_LOG_INFO);
    // ESP_LOG_BUFFER_CHAR_LEVEL(TAG, conf.ap.password, 64, ESP_LOG_INFO);
    // ESP_LOGI(TAG, "size : %d", sizeof(conf.ap.ssid));

    wifi_config_t conf_current;
    esp_wifi_get_config(WIFI_IF_AP, &conf_current);
    if (!softap_config_equal(conf, conf_current) && esp_wifi_set_config(WIFI_IF_AP, &conf) != ESP_OK)
    {
        return false;
    }

    ESP_LOGI(TAG, "AP mode started with ssid %s and passphrase %s on %d channel supporting %d connections", ssid.c_str(), passphrase.c_str(), channel, max_connection);

    return true;
}

bool WiFiAccessPoint::SoftAPConfig(IPv4Address local_ip, IPv4Address gateway, IPv4Address subnet)
{
    IP4_ADDR(&mESPNetIFInfo.ip, local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
    IP4_ADDR(&mESPNetIFInfo.gw, gateway[0], gateway[1], gateway[2], gateway[3]);
    IP4_ADDR(&mESPNetIFInfo.netmask, subnet[0], subnet[1], subnet[2], subnet[3]);

    ESP_ERROR_CHECK(esp_netif_dhcps_stop(mAPNetIF));
    ESP_ERROR_CHECK(esp_netif_set_ip_info(mAPNetIF, &mESPNetIFInfo));
    ESP_ERROR_CHECK(esp_netif_dhcps_start(mAPNetIF));

    return true;
}
