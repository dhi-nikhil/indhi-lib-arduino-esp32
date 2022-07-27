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

/**
 * @brief WIFI class
 *
 */
class WiFi : public NVStorage, public  NetworkInterface, public DefaultEventLoop, public WiFiAccessPoint, public WiFiStation, public WiFiEventGroup, public WiFiStationScan
{
private:
    static esp_event_handler_instance_t mEventHandlerInstance; /* Event handler instance */
    /* Specify the events here */

public:
    /* Methods */
    /**
     * @brief Construct a new Wi Fi object
     *
     */
    WiFi();
    /**
     * @brief Destroy the Wi Fi object
     *
     */
    ~WiFi();
    /**
     * @brief Set the Default Storage object
     *
     * @param wifiStorage
     * @return esp_err_t
     */
    static esp_err_t SetDefaultStorage(wifi_storage_t wifiStorage = WIFI_STORAGE_RAM);
    /**
     * @brief Static method Init wifi
     *
     * @return esp_err_t success or failed
     */
    static esp_err_t Init(void);
    /**
     * @brief Static method Deinit wifi
     *
     * @return esp_err_t success or failed
     */
    static esp_err_t Deinit(void);
    /**
     * @brief Static method wifi start
     *
     * @return esp_err_t success or failed
     */
    static esp_err_t Start(void);
    /**
     * @brief Static method wifi stop
     *
     * @return esp_err_t success or failed
     */
    static esp_err_t Stop(void);
    /**
     * @brief Register handler
     *
     * @return esp_err_t success or failed
     */
    esp_err_t RegisterHandler(void);
    /**
     * @brief UNregister handler
     *
     * @return esp_err_t success or failed
     */
    static esp_err_t UnregisterHandler(void);
    /**
     * @brief Get the Channel object
     *
     * @return int32_t wifi channel
     */
    static int32_t GetChannel(void);
    /**
     * @brief Set the Mode object
     *
     * @param mode wifi mode
     * @return esp_err_t success or failed
     */
    static esp_err_t SetMode(wifi_mode_t mode);
    /**
     * @brief Get the Mode object
     *
     * @return wifi_mode_t
     */
    static wifi_mode_t GetMode(void);
    /**
     * @brief Enable sta
     *
     * @param enable sta flag
     * @return esp_err_t success or failed
     */
    static esp_err_t EnableSTA(bool enable);
    /* Default for all wifi events */
    /**
     * @brief wifi event handler
     *
     * @param handlerArgument handler argument
     * @param eventBase event base
     * @param eventId event id
     * @param eventData event data
     */
    static void wifi_event_handler(void * handlerArgument, esp_event_base_t eventBase, int32_t eventId, void * eventData);
    /**
     * @brief Enable ap
     *
     * @param enable ap flag
     * @return esp_err_t success or failed
     */
    static esp_err_t EnableAP(bool enable);
};

#endif
