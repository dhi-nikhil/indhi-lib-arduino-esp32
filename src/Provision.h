#ifndef PROVISION_H_
#define PROVISION_H_

#include "WiFi.h"
#include "EventGroup.h"
#include "EventLoop.h"
#include "HTTPServer.h"
#include "HTTPClient.h"
#include "ProvisionEventLoop.h"
#include "Ping.h"
#include "BLEProvision.h"
#include "Device.h"
#include "MQTTClient.h"
#include "sys/param.h"

class Provision :public WiFi, public ProvisionEventGroup, public HTTPServer, public HTTPClient, public Ping, public MQTTClient, public BLEProvision
{
private:
    /**
     * @brief store the reference to the object Device
     *
     */
    class Device * mDevice;
    std::string mName;
    std::string mSSID;
    std::string mPSK;
    std::string mValidationEndpoint;
    std::string mInitializationEndpoint;
    std::string mConfigKey;
    std::string mIPAddress;
    std::string mAPIKey;
    std::string mMQTTURI;
    std::string mMQTTClient;
    std::string mMQTTLastWillMessage;
    std::string mDeviceSubTopic;
    std::string mDevicePubTopic;
    std::string mEncryptionKey;
    bool mMQTTConnectionVerification;
    bool mProvComplete = false;

    static esp_err_t mWifiScanRequestHandler(httpd_req_t * request);
    static esp_err_t mWifiConfidRequestHandler(httpd_req_t * request);
    static esp_err_t mWifiRequestHandler(httpd_req_t * request);
    static esp_err_t mWifiErrorHandler(httpd_req_t * request);
    static esp_err_t mInvalidHandler(httpd_req_t * request, httpd_err_code_t httpd_err_code);

    static Provision * provision;

public:
    Provision(Device * device);
    ~Provision();
    void SetValidationEndpoint(std::string validationEndpoint);
    void SetInitializationEndpoint(std::string initializationEndpoint);
    void SetConfigKey(std::string configKey);
    void Connect();
    esp_err_t StartProvisioningBLE();
    esp_err_t StartProvisioningWiFi();
    esp_err_t StartProvisioningWiFi(std::string ssid, std::string psk, std::string token);
    esp_err_t StartProvisioningMainApp();
    void EraseData();
    esp_err_t RetriveData();
    esp_err_t SaveData();
    void StopProvisioning();
    esp_err_t CreateInitializationEndpointRequestCreate(std::string * request);
    esp_err_t FetchAPIKeyFromResponse(std::string response);
    esp_err_t FetchDataFromInitResponse(std::string response);
    static void MQTTEventHandlerProvision(void * handlerArgs, esp_event_base_t base, int32_t id, void * eventData);
    static void GAPEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t * param);
    static void GATTEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t * param);
};


#endif
