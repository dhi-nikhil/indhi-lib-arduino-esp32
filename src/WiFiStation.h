#ifndef WIFI_STA_H_
#define WIFI_STA_H_

#include "WiFi.h"
#include "WiFiType.h"
#include "string"
#include "WiFiSTAEventGroup.h"
#include "esp_netif.h"
#include "esp_netif_types.h"

/**
 * @brief WiFI station scan
 *
 */
class WiFiStation : public WiFiSTAEventGroup
{
protected:
    static bool mAutoReconnect; /* auto reconnect flag */
public:
    /**
     * @brief Construct a new Wi Fi Station object
     *
     */
    WiFiStation();
    /**
     * @brief Destroy the Wi Fi Station object
     *
     */
    ~WiFiStation();
    /**
     * @brief STA configure
     *
     * @param ssid ssid of the sta
     * @param passphrase psk of the sta
     * @param channel channel of the sta
     * @param bssid bssid of the sta
     * @param connect connect flag
     * @return esp_err_t return success or failed
     */
    esp_err_t STAConfig(std::string ssid, std::string passphrase = nullptr, int32_t channel = 0, const uint8_t * bssid = nullptr, bool connect = true);
    /**
     * @brief Create
     *
     */
    static void Create();
    /**
     * @brief Delete
     *
     */
    static void Delete();
    /**
     * @brief Connect STA
     *
     * @return esp_err_t return success or failed
     */
    esp_err_t Connect();
    /**
     * @brief Reconnect STA
     *
     * @return esp_err_t return success or failed
     */
    esp_err_t Reconnect();
    /**
     * @brief Disconnect from STA
     *
     * @param wifioff turn off the sta
     * @param eraseap erase ap info
     * @return esp_err_t return success or failed
     */
    esp_err_t Disconnect(bool wifioff = false, bool eraseap = false);
    /**
     * @brief Set the Auto Reconnect object
     *
     * @param autoReconnect
     * @return true
     * @return false
     */
    bool SetAutoReconnect(bool autoReconnect);
    /**
     * @brief Get the Auto Reconnect object
     *
     * @return true
     * @return false
     */
    bool GetAutoReconnect();
    /**
     * @brief MAC ID to set
     *
     * @param mac
     * @return uint8_t*
     */
    uint8_t * MACAddress(uint8_t * mac);
    /**
     * @brief Get MAC id
     *
     * @return std::string
     */
    std::string MACAddress();
    /**
     * @brief SSID of connected router
     *
     * @return std::string
     */
    std::string SSID() const;
    /**
     * @brief PSK of connected router
     *
     * @return std::string
     */
    std::string PSK() const;
    /**
     * @brief RSSI of the connected touter
     *
     * @return int8_t router rssi
     */
    int8_t RSSI();
};


#endif
