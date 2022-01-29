#ifndef WIFI_STA_H_
#define WIFI_STA_H_

#include "WiFi.h"
#include "WiFiType.h"
#include "string"
#include "WiFiSTAEventGroup.h"
#include "esp_netif.h"
#include "esp_netif_types.h"

// class WiFiStation
class WiFiStation : public WiFiSTAEventGroup
{
protected:
    static bool mAutoReconnect;
public:
    WiFiStation();
    ~WiFiStation();
    esp_err_t STAConfig(std::string ssid, std::string passphrase = nullptr, int32_t channel = 0, const uint8_t * bssid = nullptr, bool connect = true);
    static void Create();
    static void Delete();
    esp_err_t Connect();
    esp_err_t Reconnect();
    esp_err_t Disconnect(bool wifioff = false, bool eraseap = false);
    bool SetAutoReconnect(bool autoReconnect);
    bool GetAutoReconnect();
    uint8_t * MACAddress(uint8_t * mac);
    std::string MACAddress();
    std::string SSID() const;
    std::string PSK() const;
    int8_t RSSI();
};


#endif
