#ifndef WIFI_H_
#define WIFI_H_

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "NetworkInterface.h"
#include "WiFiStation.h"
#include "WiFiAccessPoint.h"
#include "NVStorage.h"
#include "DefaultEventLoop.h"
#include "WiFiEventGroup.h"
#include "WiFiStationScan.h"

class WiFi : public NVStorage, public  NetworkInterface, public DefaultEventLoop, public WiFiAccessPoint, public WiFiStation, public WiFiEventGroup, public WiFiStationScan
{
private:
    static esp_event_handler_instance_t mEventHandlerInstance;
    /* Specify the events here */

public:
    /* Methods */
    WiFi();
    ~WiFi();
    static esp_err_t SetDefaultStorage(wifi_storage_t wifiStorage = WIFI_STORAGE_RAM);
    static esp_err_t Init(void);
    static esp_err_t Deinit(void);
    static esp_err_t Start(void);
    static esp_err_t Stop(void);
    esp_err_t RegisterHandler(void);
    static esp_err_t UnregisterHandler(void);
    static int32_t GetChannel(void);
    static esp_err_t SetMode(wifi_mode_t mode);
    static wifi_mode_t GetMode(void);
    static esp_err_t EnableSTA(bool enable);
    /* Default for all wifi events */
    static void wifi_event_handler(void * handlerArgument, esp_event_base_t eventBase, int32_t eventId, void * eventData);
    static esp_err_t EnableAP(bool enable);
};

#endif
