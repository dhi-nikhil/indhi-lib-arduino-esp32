#ifndef PROVISION_MAIN_APP_H_
#define PROVISION_MAIN_APP_H_

#include "WiFi.h"
#include "EventGroup.h"
#include "EventLoop.h"
#include "HTTPServer.h"
#include "HTTPClient.h"
#include "ProvisionEventLoop.h"
#include "Ping.h"
#include "DeviceMainApp.h"
#include "MQTTClient.h"
#include "sys/param.h"

class ProvisionMainApp :public WiFi, public ProvisionEventGroup, public HTTPServer, public HTTPClient, public Ping, public MQTTClient
{
private:
    class DeviceMainApp * mDevice;

    std::string mName;
    /* Fetched from HTTP Server */
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
    bool mProvComplete = false;

    static esp_err_t mWifiScanRequestHandler(httpd_req_t * request);
    static esp_err_t mWifiConfidRequestHandler(httpd_req_t * request);
    static esp_err_t mWifiRequestHandler(httpd_req_t * request);
    static esp_err_t mWifiErrorHandler(httpd_req_t * request);
    static esp_err_t mInvalidHandler(httpd_req_t * request, httpd_err_code_t httpd_err_code);

public:
    ProvisionMainApp(DeviceMainApp * device);
    ~ProvisionMainApp();
    void SetValidationEndpoint(std::string validationEndpoint)
    {
        mValidationEndpoint = validationEndpoint;
    }
    void SetInitializationEndpoint(std::string initializationEndpoint)
    {
        mInitializationEndpoint = initializationEndpoint;
    }
    void SetConfigKey(std::string configKey)
    {
        mConfigKey = configKey;
    }
    void Connect();
    esp_err_t StartProvisioningWiFi();
    esp_err_t StartProvisioningWiFi(std::string ssid, std::string psk, std::string token);
    esp_err_t StartProvisioningMainApp();
    void EraseData();
    esp_err_t RetriveData();
    esp_err_t SaveData();
    void StopProvisioning();
    esp_err_t CreateValidationEndpointRequestCreate(std::string * request);
    esp_err_t FetchAPIKeyFromResponse(std::string response);
    esp_err_t FetchDataFromValidationResponse(std::string response);
    static void MQTTEventHandlerProvision(void * handlerArgs, esp_event_base_t base, int32_t id, void * eventData);

};


#endif
