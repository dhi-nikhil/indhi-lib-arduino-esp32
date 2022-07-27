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

/**
 * @brief Provisioning errors
 *
 */
enum
{
    PROVISIONING_OK = 0, /* Provisioning ok */
    PROVISIONING_ERROR_ABORTED_FROM_APP, /* provisioning aborted from app */
    PROVISIONING_ERROR_INCOMPLETE, /* provisioning incomplete */
    PROVISIONING_ERROR_ACTIVATION_TIMEOUT, /* provisioning activationn timeout */
    PROVISIONING_ERROR_HTTP_SERVER, /* provisioning http server error */
    PROVISIONING_ERROR_HTTP_CLIENT, /* provisioning http client error */
    PROVISIONING_ERROR_HTTP_CLIENT_VALIDATION_ENDPOINT, /* provisioning http client validation endpoint */
    PROVISIONING_ERROR_MQTT_CLIENT, /* provisioning mqtt client error */
    PROVISIONING_ERROR_WIFI_STATION_FAILED_TO_CONNECT, /* wifi station failed to connect */
    PROVISIONING_ERROR_SAVING_DATA, /* provisionig save data error */
    PROVISIONING_ERROR_INVALID, /* provisioning invalid error */
    PROVISIONING_ERROR_MAX,
};

/**
 * @brief Provisioning class
 *
 */
class Provision :public WiFi, public ProvisionEventGroup, public HTTPServer, public HTTPClient, public Ping, public MQTTClient, public BLEProvision
{
private:
    class Device * mDevice; /* store the reference to the object Device */
    std::string mName; /* name of the provisioning */
    std::string mSSID; /* ssid of the ap */
    std::string mPSK; /* psk of the ap */
    std::string mValidationEndpoint; /* validation endpoint */
    std::string mInitializationEndpoint; /* Initialization endpoint */
    std::string mConfigKey; /* Config key */
    std::string mIPAddress; /* ip address */
    std::string mAPIKey; /* api key */
    std::string mMQTTURI; /* mqtt uri */
    std::string mMQTTClient; /* mqtt client */
    std::string mMQTTLastWillMessage; /* mqtt last will message */
    std::string mDeviceSubTopic; /* device sub topic */
    std::string mDevicePubTopic; /* device pub topic */
    std::string mDataSubTopic; /* device data sub topic */
    std::string mDataPubTopic; /* device data pub topic */
    std::string mEncryptionKey; /* enccryption key */
    bool mMQTTConnectionVerification; /* mqtt connection verification */
    bool mProvComplete = false; /* provisioning complete flag */

    /**
     * @brief AP mode HTTP server scan event handler
     *
     * @param request http request
     * @return esp_err_t return if success or failed
     */
    static esp_err_t mWifiScanRequestHandler(httpd_req_t * request);
    /**
     * @brief AP mode HTTP server scan config event handler
     *
     * @param request http request
     * @return esp_err_t return if success or failed
     */
    static esp_err_t mWifiConfidRequestHandler(httpd_req_t * request);
    /**
     * @brief AP mode HTTP server wifi config event handler
     *
     * @param request http request
     * @return esp_err_t return if success or failed
     */
    static esp_err_t mWifiRequestHandler(httpd_req_t * request);
    /**
     * @brief AP mode HTTP server error event handler
     *
     * @param request http request
     * @return esp_err_t return if success or failed
     */
    static esp_err_t mWifiErrorHandler(httpd_req_t * request);
    /**
     * @brief AP mode HTTP server invalid request event handler
     *
     * @param request http request
     * @return esp_err_t return if success or failed
     */
    static esp_err_t mInvalidHandler(httpd_req_t * request, httpd_err_code_t httpd_err_code);

    static Provision * provision; /* store  the reference to the object */

public:
    /**
     * @brief Construct a new Provision object
     *
     *
     * @param device device to provision
     */
    Provision(Device * device);
    /**
     * @brief Destroy the Provision object
     *
     */
    ~Provision();
    /**
     * @brief Set the Validation Endpoint object
     *
     * @param validationEndpoint validation endpoint to set
     */
    void SetValidationEndpoint(std::string validationEndpoint);
    /**
     * @brief Set the Initialization Endpoint object
     *
     * @param initializationEndpoint init endpoint to set
     */
    void SetInitializationEndpoint(std::string initializationEndpoint);
    /**
     * @brief Set the Config Key object
     *
     * @param configKey config key to set
     */
    void SetConfigKey(std::string configKey);
    /**
     * @brief Connect to the backend
     *
     * @return esp_err_t return if success or not
     */
    esp_err_t Connect();
    /**
     * @brief Start provisioning using BLE
     *
     * @return esp_err_t return if success or not
     */
    esp_err_t StartProvisioningBLE();
    /**
     * @brief Start provisioning using WiFi
     *
     * @return esp_err_t return if success or not
     */
    esp_err_t StartProvisioningWiFi();
    /**
     * @brief Start provisioning using token
     *
     * @param ssid ssid of the wifi
     * @param psk psk of the wifi
     * @param token token to connect with
     * @return esp_err_t return if success or not
     */
    esp_err_t StartProvisioningWiFi(std::string ssid, std::string psk, std::string token);
    /**
     * @brief Erase all data
     *
     */
    void EraseData();
    /**
     * @brief Retrive data
     *
     * @return esp_err_t return if success or not
     */
    esp_err_t RetriveData();
    /**
     * @brief Save data
     *
     * @return esp_err_t return if success or not
     */
    esp_err_t SaveData();
    /**
     * @brief Stop provisioning
     *
     */
    void StopProvisioning();
    /**
     * @brief Create a Initialization Endpoint Request Create object
     *
     * @param request store the results here
     * @return esp_err_t return if success or not
     */
    esp_err_t CreateInitializationEndpointRequestCreate(std::string * request);
    /**
     * @brief Fetch the API key from response
     *
     * @param response fetch key from this response
     * @return esp_err_t return if success or not
     */
    esp_err_t FetchAPIKeyFromResponse(std::string response);
    /**
     * @brief Fetch data from init response
     *
     * @param response Init response
     * @return esp_err_t return if success or not
     */
    esp_err_t FetchDataFromInitResponse(std::string response);
    /**
     * @brief Static MQTT event handler
     *
     * @param handlerArgs event handler data
     * @param base event base
     * @param id event id
     * @param eventData event data
     */
    static void MQTTEventHandlerProvision(void * handlerArgs, esp_event_base_t base, int32_t id, void * eventData);
    /**
     * @brief Static GAP event handler
     *
     * @param event event paramters
     * @param param data paramters
     */
    static void GAPEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t * param);
    /**
     * @brief Static GAT event handler
     *
     * @param event event paramters
     * @param gatts_if gatt if
     * @param param data paramters
     */
    static void GATTEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t * param);
};


#endif
