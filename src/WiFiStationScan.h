#ifndef WIFI_SCAN_H_
#define WIFI_SCAN_H_

#include "WiFiScanEventGroup.h"
#include "esp_wifi_types.h"

class WiFiStationScan : public WiFiScanEventGroup
{
private:
    wifi_ap_record_t * mWiFiAPRecord;

protected:
public:
    enum WiFiScanStatus
    {
        SCAN_IDLE,
        SCAN_STARTED,
        SCAN_COMPLETED,
        SCAN_INPROCESS,
        SCAN_ERROR,
        SCAN_MAX,
    };
    WiFiScanStatus Scan(bool block);
    WiFiScanStatus IsScanComplete();
    uint16_t GetScanEntries(uint16_t entries = 10);
    std::string SSID(uint16_t index);
    int8_t RSSI(uint16_t index);
    std::string EncryptionType(uint16_t index);
    // esp_err_t ScanDelete;
    /* Fetch */
    // esp_err_t SSID;
    // esp_err_t RSSI;
    // esp_err_t BSSI;
    // esp_err_t Channel;

};


#endif
