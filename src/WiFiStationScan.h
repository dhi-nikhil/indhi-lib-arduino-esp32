#ifndef WIFI_SCAN_H_
#define WIFI_SCAN_H_

#include "WiFiScanEventGroup.h"
#include "esp_wifi_types.h"

/**
 * @brief WiFi station scan class
 *
 */
class WiFiStationScan : public WiFiScanEventGroup
{
private:
    wifi_ap_record_t * mWiFiAPRecord; /* wifi ap records */

protected:
public:
    /**
     * @brief WiFi scan status
     *
     */
    enum WiFiScanStatus
    {
        SCAN_IDLE, /* scan status idle */
        SCAN_STARTED, /* scan started */
        SCAN_COMPLETED, /* scan completed */
        SCAN_INPROCESS, /* scan inprogress */
        SCAN_ERROR, /* scan error */
        SCAN_MAX,
    };
    /**
     * @brief WiFi scan
     *
     * @param block blocking operation or not
     * @return WiFiScanStatus scan status
     */
    WiFiScanStatus Scan(bool block);
    /**
     * @brief Get the scan complete status
     *
     * @return WiFiScanStatus scan status
     */
    WiFiScanStatus IsScanComplete();
    /**
     * @brief Get the Scan Entries object
     *
     * @param entries Entries to fetch
     * @return uint16_t return the number of entries in the result
     */
    uint16_t GetScanEntries(uint16_t entries = 10);
    /**
     * @brief SSID of the scan result
     *
     * @param index index of the scan result
     * @return std::string SSID
     */
    std::string SSID(uint16_t index);
    /**
     * @brief RSSI of the scan result
     *
     * @param index index of the scan result
     * @return int8_t RSSI
     */
    int8_t RSSI(uint16_t index);
    /**
     * @brief Encryption type
     *
     * @param index index of the scan result
     * @return std::string encryption type
     */
    std::string EncryptionType(uint16_t index);
    // esp_err_t ScanDelete;
    /* Fetch */
    // esp_err_t SSID;
    // esp_err_t RSSI;
    // esp_err_t BSSI;
    // esp_err_t Channel;

};


#endif
