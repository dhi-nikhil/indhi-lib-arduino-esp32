#ifndef WIFI_AP_H_
#define WIFI_AP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "IPv4Address.h"
#include "esp_wifi_types.h"
#include "string"
#include "string.h"
#include "WiFi.h"
#include "WiFiAPEventGroup.h"

class WiFiAccessPoint : public WiFiAPEventGroup
{
private:
    esp_netif_ip_info_t mESPNetIFInfo;
public:
    WiFiAccessPoint();
    ~WiFiAccessPoint();
    void Create();
    void Delete();
    bool SoftAPConfig(std::string ssid, std::string passphrase, int channel = 1, int ssid_hidden = 0, int max_connection = 1);
    bool SoftAPConfig(IPv4Address local_ip, IPv4Address gateway, IPv4Address subnet);
    // bool softAPdisconnect(bool wifioff = false);
    // uint8_t softAPgetStationNum();
    // IPAddress softAPIP();
    // IPAddress softAPBroadcastIP();
    // IPAddress softAPNetworkID();
    // uint8_t softAPSubnetCIDR();
    // bool softAPenableIpV6();
    // const char * softAPgetHostname();
    // bool softAPsetHostname(const char * hostname);
    // uint8_t * softAPmacAddress(uint8_t * mac);
    // std::string softAPmacAddress(void);
    // std::string softAPSSID(void) const;
};


#endif
