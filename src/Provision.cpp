#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "Provision.h"
#include "WiFi.h"
#include "EventLoop.h"
#include "EventGroup.h"
#include "ProvisionEventLoop.h"
#include "cJSON.h"
#include "System.h"
#include "Utils.h"
#include "Device.h"
#include "DeviceConfig.h"
#include "string"

static const char * TAG = "Provision";

Provision * Provision::provision = nullptr;

esp_err_t Provision::mWifiScanRequestHandler(httpd_req_t * request)
{
    cJSON * obj, * arr;
    char * str;
    uint16_t scanEntries;
    Provision * provision = (Provision *) request->user_ctx;
    WiFiScanStatus wifiScanStatus = provision->Scan(true);
    scanEntries = provision->GetScanEntries();
    // wifiScanStatus = WiFiScanStatus::SCAN_ERROR;
    // scanEntries = 0;
    obj = cJSON_CreateObject();
    System::PrintMinimumFreeHeapSize();
    if (wifiScanStatus != WiFiScanStatus::SCAN_COMPLETED || scanEntries == 0)
    {
        cJSON_AddNumberToObject(obj, "code", 400);
        cJSON_AddStringToObject(obj, "message", "Failed to scan network");
        cJSON_AddStringToObject(obj, "name", "Failed to scan network");
        httpd_resp_set_status(request, HTTPD_400);
        System::PrintMinimumFreeHeapSize();
    }
    else
    {
        cJSON_AddNumberToObject(obj, "code", 200);
        cJSON_AddStringToObject(obj, "message", "Configuration Received");
        arr = cJSON_AddArrayToObject(obj, "data");
        ESP_LOGI(TAG, "WiFi scan entries : %d", scanEntries);
        for (int i = 0; i < scanEntries; i++)
        {
            cJSON * entry;
            entry = cJSON_CreateObject();
            cJSON_AddStringToObject(entry, "ssid", provision->WiFiStationScan::SSID(i).c_str());
            cJSON_AddStringToObject(entry, "auth", provision->WiFiStationScan::EncryptionType(i).c_str());
            cJSON_AddNumberToObject(entry, "rssi", provision->WiFiStationScan::RSSI(i));
            cJSON_AddItemToArray(arr, entry);
            ESP_LOGI(TAG, "SSID : %s, RSSI : %d, Encryption Type : %s", provision->WiFiStationScan::SSID(i).c_str(), provision->WiFiStationScan::RSSI(i), provision->WiFiStationScan::EncryptionType(i).c_str());
        }
        httpd_resp_set_status(request, HTTPD_200);
    }

    str = cJSON_PrintUnformatted(obj);
    httpd_resp_send(request, str, HTTPD_RESP_USE_STRLEN);
    cJSON_Delete(obj);
    free(str);
    return ESP_OK;
}

esp_err_t Provision::mWifiRequestHandler(httpd_req_t * request)
{
    cJSON * obj, * arr;
    char * str;
    Provision * provision = (Provision *) request->user_ctx;
    /* Fetch ssid and password and connect using WiFiStation */
    esp_err_t err;

    char * requestBuffer; //  max size
    char featchedData[100] = { 0 };
    char ssidBuffer[33] = { 0 };
    char pskBuffer[65] = { 0 };
    char ssidDecodedBuffer[33] = { 0 };
    char pskDecodedBuffer[65] = { 0 };

    memset(&featchedData[0], 0, 100);
    memset(&ssidBuffer[0], 0, 33);
    memset(&pskBuffer[0], 0, 65);
    memset(&ssidDecodedBuffer[0], 0, 33);
    memset(&pskDecodedBuffer[0], 0, 65);

    std::string ssid;
    std::string psk;
    size_t remaining;

    remaining = request->content_len;
    requestBuffer = (char *) calloc(remaining + 1, sizeof(char));
    httpd_req_recv(request, requestBuffer, remaining);
    ESP_LOGI(TAG, "Buffer : %s", requestBuffer);

    err = ( httpd_query_key_value(requestBuffer, "ssid", ssidBuffer, 32) || httpd_query_key_value(requestBuffer, "psk", pskBuffer, 64) );
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    Utils::URLDecode(&ssidBuffer[0], &ssidDecodedBuffer[0]);
    Utils::URLDecode(&pskBuffer[0], &pskDecodedBuffer[0]);

    ssid = std::string(ssidDecodedBuffer);
    psk = std::string(pskDecodedBuffer);

    obj = cJSON_CreateObject();

    if (err != ESP_OK)
    {
        cJSON_AddNumberToObject(obj, "code", 400);
        cJSON_AddStringToObject(obj, "message", "Data missing");
        cJSON_AddStringToObject(obj, "name", "Failed due to missing data");
        ESP_LOGE(TAG, "Error while fetching SSID or Password");
        httpd_resp_set_status(request, HTTPD_400);
    }
    else
    {
        ESP_LOGI(TAG, "Fetched SSID : %s", ssid.c_str());
        ESP_LOGI(TAG, "Fetched PSK : %s", psk.c_str());

        provision->WiFiStation::Disconnect();
        vTaskDelay(pdMS_TO_TICKS(100)); // so the states are saved properly
        // provision->WiFiSTAEventGroup::EventGroupWaitBits(WiFiSTAEventGroup::EG_STA_DISCONNECT, false, false, portMAX_DELAY); // so the states are saved properly
        provision->STAConfig(ssid, psk, 0, nullptr, true);

        // provision->WiFiSTAEventGroup::EventGroupSetBits(WiFiSTAEventGroup::EG_STA_CONNECTED);
        provision->WiFiSTAEventGroup::EventGroupWaitBits(WiFiSTAEventGroup::EG_STA_CONNECTED, false, false, pdMS_TO_TICKS(10000));
        if (provision->WiFiSTAEventGroup::IsEventGroupBitSet(WiFiSTAEventGroup::EG_STA_CONNECTED))
        {
            cJSON_AddNumberToObject(obj, "code", 200);
            cJSON_AddStringToObject(obj, "message", "Connected to the Router successfully");
            ESP_LOGI(TAG, "STA connection success");
            provision->mSSID = ssid;
            provision->mPSK = psk;
            httpd_resp_set_status(request, HTTPD_200);
        }
        else
        {
            cJSON_AddNumberToObject(obj, "code", 400);
            cJSON_AddStringToObject(obj, "message", "Failed to connect");
            cJSON_AddStringToObject(obj, "name", "Failed to connect to the router");
            ESP_LOGI(TAG, "STA connection failed");
            httpd_resp_set_status(request, HTTPD_400);
        }
    }

    str = cJSON_PrintUnformatted(obj);
    httpd_resp_send(request, str, HTTPD_RESP_USE_STRLEN);
    cJSON_Delete(obj);
    free(str);
    return ESP_OK;
}

esp_err_t Provision::mWifiConfidRequestHandler(httpd_req_t * request)
{
    cJSON * obj, * arr;
    char * str;
    Provision * provision = (Provision *) request->user_ctx;
    /* Fetch ssid and password and connect using WiFiStation */
    esp_err_t err;

    char * requestBuffer; //  max size
    char featchedData[100] = { 0 };
    char validationEndpointBuffer[100] = { 0 };
    char initializationEndpointBuffer[100] = { 0 };
    char configKeyBuffer[50] = { 0 };
    char ipAddressBuffer[50] = { 0 };
    char validationEndpointDecodedBuffer[100] = { 0 };
    char initializationEndpointDecodedBuffer[100] = { 0 };
    char configKeyDecodedBuffer[50] = { 0 };
    char ipAddressDecodedBuffer[50] = { 0 };

    memset(&featchedData[0], 0, 100);
    memset(&validationEndpointBuffer[0], 0, 100);
    memset(&initializationEndpointBuffer[0], 0, 100);
    memset(&configKeyBuffer[0], 0, 50);
    memset(&ipAddressBuffer[0], 0, 50);
    memset(&validationEndpointDecodedBuffer[0], 0, 100);
    memset(&initializationEndpointDecodedBuffer[0], 0, 100);
    memset(&configKeyDecodedBuffer[0], 0, 50);
    memset(&ipAddressDecodedBuffer[0], 0, 50);

    std::string initializationEndpoint;
    std::string validationEndpoint;
    std::string configKey;
    std::string ipAddress;

    size_t remaining;

    remaining = request->content_len;
    requestBuffer = (char *) calloc(remaining + 1, sizeof(char));
    httpd_req_recv(request, requestBuffer, remaining);
    ESP_LOGI(TAG, "Buffer : %s", requestBuffer);

    err = ( httpd_query_key_value(requestBuffer, "device_validation_endpoint", validationEndpointBuffer, 100) ||
           httpd_query_key_value(requestBuffer, "device_initialization_endpoint", initializationEndpointBuffer, 100) ||
           httpd_query_key_value(requestBuffer, "validation_token", configKeyBuffer, 50) ||
           httpd_query_key_value(requestBuffer, "server_ip_address", ipAddressBuffer, 50) );
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    Utils::URLDecode(&validationEndpointBuffer[0], &validationEndpointDecodedBuffer[0]);
    Utils::URLDecode(&initializationEndpointBuffer[0], &initializationEndpointDecodedBuffer[0]);
    Utils::URLDecode(&configKeyBuffer[0], &configKeyDecodedBuffer[0]);
    Utils::URLDecode(&ipAddressBuffer[0], &ipAddressDecodedBuffer[0]);

    validationEndpoint = std::string(validationEndpointDecodedBuffer);
    initializationEndpoint = std::string(initializationEndpointDecodedBuffer);
    configKey = std::string(configKeyDecodedBuffer);
    ipAddress = std::string(ipAddressDecodedBuffer);

    obj = cJSON_CreateObject();

    if (err != ESP_OK)
    {
        cJSON_AddNumberToObject(obj, "code", 400);
        cJSON_AddStringToObject(obj, "message", "Data missing");
        cJSON_AddStringToObject(obj, "name", "Failed due to missing data");
        ESP_LOGE(TAG, "Error while fetching config key or ip address or validation endpoint");
        httpd_resp_set_status(request, HTTPD_400);
    }
    else
    {
        ESP_LOGI(TAG, "Fetched validationEndpoint : %s", validationEndpoint.c_str());
        ESP_LOGI(TAG, "Fetched configKey : %s", configKey.c_str());
        ESP_LOGI(TAG, "Fetched ipAddress : %s", ipAddress.c_str());

        /* Ping here */
        PingStatus pingStatus = provision->PingIPv4Address(ipAddress, 6000);
        ESP_LOGI(TAG, "Ping status : %d", pingStatus);
        // if (provision->PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_SUCCESS))
        if (provision->PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_SUCCESS))
        {
            cJSON_AddNumberToObject(obj, "code", 200);
            cJSON_AddStringToObject(obj, "message", "Configuration received");
            ESP_LOGI(TAG, "Ping success and configuration saved");
            provision->ProvisionEventGroup::EventGroupSetBits(ProvisionEventGroup::EG_PROVISION_AP_HTTP_SERVER_COMPLETED);
            provision->mValidationEndpoint = validationEndpoint;
            provision->mInitializationEndpoint = initializationEndpoint;
            provision->mConfigKey = configKey;
            provision->mIPAddress = ipAddress;
            httpd_resp_set_status(request, HTTPD_200);
        }
        else
        {
            cJSON_AddNumberToObject(obj, "code", 400);
            cJSON_AddStringToObject(obj, "message", "Ping failed");
            cJSON_AddStringToObject(obj, "name", "Failed to ping the server");
            ESP_LOGI(TAG, "Ping failed");
            httpd_resp_set_status(request, HTTPD_400);
        }
    }

    str = cJSON_PrintUnformatted(obj);
    httpd_resp_send(request, str, HTTPD_RESP_USE_STRLEN);
    cJSON_Delete(obj);
    free(str);
    return ESP_OK;
}

esp_err_t Provision::mWifiErrorHandler(httpd_req_t * request)
{
    cJSON * obj, * arr;
    char * str;
    Provision * provision = (Provision *) request->user_ctx;
    /* Fetch ssid and password and connect using WiFiStation */
    esp_err_t err;

    obj = cJSON_CreateObject();

    provision->ProvisionEventGroup::EventGroupSetBits(ProvisionEventGroup::EG_PROVISION_ABORTED);
    if (provision->ProvisionEventGroup::IsEventGroupBitSet(ProvisionEventGroup::EG_PROVISION_ABORTED))
    {

        httpd_resp_send(request, "{\"code\":200,\"message\":\"Request received success\"}", HTTPD_RESP_USE_STRLEN);
        cJSON_AddNumberToObject(obj, "code", 200);
        cJSON_AddStringToObject(obj, "message", "Request received success");
        httpd_resp_set_status(request, HTTPD_400);
    }
    else
    {
        cJSON_AddNumberToObject(obj, "code", 400);
        cJSON_AddStringToObject(obj, "message", "Failed to process request");
        cJSON_AddStringToObject(obj, "name", "Failed to process request");
        ESP_LOGE(TAG, "Provisioning failed to aborted from mobile application");
        httpd_resp_set_status(request, HTTPD_400);
    }

    str = cJSON_PrintUnformatted(obj);
    httpd_resp_send(request, str, HTTPD_RESP_USE_STRLEN);
    cJSON_Delete(obj);
    free(str);
    return ESP_OK;

    return ESP_OK;
}

esp_err_t Provision::mInvalidHandler(httpd_req_t * request, httpd_err_code_t httpd_err_code)
{
    ESP_LOGI(TAG, "uri : %s", (char *) request->uri);
    return ESP_OK;
}

Provision::Provision(Device * device) : WiFi(), ProvisionEventGroup(), HTTPServer(), HTTPClient(), Ping(), MQTTClient()
{
    mDevice = device;
    provision = this;
    NVStorage::Open("Provision");
    ProvisionEventGroup::EventGroupSetBits(ProvisionEventGroup::EG_PROVISION_IDLE);
    WiFiScanEventGroup::EventGroupSetBits(EG_SCAN_IDLE);
    WiFiSTAEventGroup::EventGroupSetBits(EG_STA_IDLE);
    PingEventGroup::EventGroupSetBits(EG_PING_IDLE);
}

Provision::~Provision()
{
}

void Provision::SetValidationEndpoint(std::string validationEndpoint)
{
    mValidationEndpoint = validationEndpoint;
}

void Provision::SetInitializationEndpoint(std::string initializationEndpoint)
{
    mInitializationEndpoint = initializationEndpoint;
}

void Provision::SetConfigKey(std::string configKey)
{
    mConfigKey = configKey;
}

void Provision::MQTTEventHandlerProvision(void * handlerArgs, esp_event_base_t base, int32_t id, void * eventData)
{
    Provision * provision = (Provision *) handlerArgs;
    esp_mqtt_event_t * mqttData = (esp_mqtt_event_t *) eventData;
    ESP_LOGI(__func__, "Event Base : %s, Event Id : %d", base, id);
    switch (id)
    {
        case MQTT_EVENT_ANY:
            break;
        case MQTT_EVENT_ERROR: /*!< on error event, additional context: connection return code, error handle from esp_tls (if supported) */
            break;
        case MQTT_EVENT_CONNECTED: /*!< connected event, additional context: session_present flag */
        {
            provision->MQTTClient::Subscribe(provision->mDeviceSubTopic);
            provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_MQTT_EVENT_CONNECTED);
            provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_MQTT_EVENT_DISCONNECTED);
            provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_ACTIVATION_PENDING);
        }
        break;
        case MQTT_EVENT_DISCONNECTED: /*!< disconnected event */
            provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_MQTT_EVENT_CONNECTED);
            provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_MQTT_EVENT_DISCONNECTED);
            break;
        case MQTT_EVENT_SUBSCRIBED: /*!< subscribed event, additional context: msg_id */
        {
        }
        break;
        case MQTT_EVENT_UNSUBSCRIBED: /*!< unsubscribed event */
        {
        }
        break;
        case MQTT_EVENT_PUBLISHED: /*!< published event, additional context:  msg_id */
        {
        }
        break;
        case MQTT_EVENT_DATA: /*!< data event, additional context: */
        {
            std::string data = provision->mDevice->DecryptPayload(std::string(mqttData->data, mqttData->data_len));
            ESP_LOGI(TAG, "Message %s", data.c_str());
            
            /* Send Activation success here */
            if (provision->mDevice->GetCommandType(data) == provision->mDevice->CMD_ACTIVATION && provision->mDevice->GetCommandValue(data) == provision->mDevice->ACTIVATION_SUCCESS)
            {
                provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_ACTIVATION_SUCCESS);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_FAILED);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_PENDING);
                provision->MQTTClient::Send(provision->mDevicePubTopic, provision->mDevice->CreateCmd(provision->mDevice->CMD_CONNECTION,provision->mDevice->CONNECTED), 0, true);
            }
            else
            {
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_SUCCESS);
                provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_ACTIVATION_FAILED);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_PENDING);
                provision->MQTTClient::Send(provision->mDevicePubTopic, provision->mDevice->CreateCmd(provision->mDevice->CMD_ACTIVATION,provision->mDevice->ACTIVATION_FAILED), 0, true);
            }
        }
        break;
        case MQTT_EVENT_BEFORE_CONNECT: /*!< The event occurs before connecting */
            break;
        case MQTT_EVENT_DELETED: /*!< Notification on delete of one message from the internal outbox */
            break;
        default:
            break;
    }
}

esp_err_t Provision::Connect()
{
    if (!mProvComplete)
        return ESP_FAIL;
    EnableSTA(true);
    SetAutoReconnect(true);
    STAConfig(mSSID, mPSK, 0, nullptr, true);

    WiFiSTAEventGroup::EventGroupWaitBits(WiFiSTAEventGroup::EG_STA_CONNECTED, false, false, portMAX_DELAY);
    if (WiFiSTAEventGroup::IsEventGroupBitSet(WiFiSTAEventGroup::EG_STA_CONNECTED))
    {
        ESP_LOGI(TAG, "STA connection success");
    }
    else
    {
        ESP_LOGE(TAG, "STA connection failed");
        return ESP_FAIL;
    }

    vTaskDelay(pdMS_TO_TICKS(2000));

    std::string response;
    HTTPClient::HTTPClientInit();
    HTTPClient::HTTPClientSetURL(mInitializationEndpoint);
    HTTPClient::HTTPClientSetMethod(HTTP_METHOD_POST);
    HTTPClient::HTTPClientSetHeader("X-API-KEY", mAPIKey);
    HTTPClient::HTTPClientSetHeader("Content-Type", "application/json");

    std::string initRequest;
    if (CreateInitializationEndpointRequestCreate(&initRequest,mTemplateVersion) != ESP_OK)
    {
        ESP_LOGE(TAG, "Create Initialization Endpoint Request Failed");
        return ESP_FAIL;
    }
    /**/
    ESP_LOGI(TAG, "Init Request %s", initRequest.c_str());
    ESP_ERROR_CHECK_WITHOUT_ABORT(HTTPClient::HTTPClientSetPostField(&initRequest));
    if (HTTPClient::HTTPClientPerform() != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to perform");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Response Content Length : %d", HTTPClient::HTTPClientGetContentLength());
    ESP_LOGI(TAG, "Response Status Code : %d", HTTPClient::HTTPClientGetStatusCode());

    HTTPClient::HTTPClientReadResponse(&response);
    ESP_LOGI(TAG, "Response : %s", response.c_str());
    if (HTTPClient::HTTPClientGetStatusCode() >= 500 && HTTPClient::HTTPClientGetStatusCode() < 600)
    {
        ESP_LOGE(TAG, "Error HTTP Client due to server side error");
        return ESP_FAIL;
    }
    else if (HTTPClient::HTTPClientGetStatusCode() >= 400 && HTTPClient::HTTPClientGetStatusCode() < 500)
    {
        ESP_LOGE(TAG, "Error HTTP Client due to client side error");
        EraseData();
        return ESP_FAIL;
    }

    HTTPClient::HTTPClientCleanup();

    FetchDataFromInitResponse(response);
    if (mMQTTURI.empty() || mMQTTClient.empty() || mDeviceSubTopic.empty() || mDevicePubTopic.empty() || mDataPubTopic.empty() || mDataSubTopic.empty())
    {
        ESP_LOGE(TAG, "Fetch data from Init response failed");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "MQTT URI : %s", mMQTTURI.c_str());
    ESP_LOGI(TAG, "MQTT Client ID : %s", mMQTTClient.c_str());
    ESP_LOGI(TAG, "Device Sub Topic : %s", mDeviceSubTopic.c_str());
    ESP_LOGI(TAG, "Device Pub Topic : %s", mDevicePubTopic.c_str());
    ESP_LOGI(TAG, "Encryption Key : %s", mEncryptionKey.c_str());

    if (!mEncryptionKey.empty())
    {
        PayloadEncryption::Init();
        PayloadEncryption::SetEncryptionKey(mEncryptionKey);
    }
    mDevice->mMQTTURI = mMQTTURI;
    mDevice->mMQTTClient = mMQTTClient;
    mDevice->mDevicePubTopic = mDevicePubTopic;
    mDevice->mDeviceSubTopic = mDeviceSubTopic;
    mDevice->mDataPubTopic = mDataPubTopic;
    mDevice->mDataSubTopic = mDataSubTopic;

    mMQTTLastWillMessage = std::string(mDevice->CreateCmd(mDevice->CMD_CONNECTION,mDevice->DISCONNECTED));
    ESP_LOGI(TAG, "Disconnected message : %s", mMQTTLastWillMessage.c_str());
    mDevice->MQTTClient::Uri(mMQTTURI)->ClientID(mMQTTClient)->Handler(mDevice->MQTTEventHandlerDevice)->Context(mDevice)->LastWillMessage(mMQTTLastWillMessage)->LastWillTopic(mDevicePubTopic)->LastWillRetain(true)->LastWillQOS(2)->Keepalive(5)->TaskPriority(3)->Init()->Register(MQTT_EVENT_ANY, mDevice)->Start();
    return ESP_OK;
}

esp_err_t Provision::StartProvisioningWiFi(std::string name)
{
    if (Provision::RetriveData() == ESP_OK)
    {
        ESP_LOGI(TAG, "Retrived Data success");
        mProvComplete = true;
        WiFi::EnableAP(false);
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG, "Retrived Data failed");
    }

    IPv4Address ipAddress(192, 168, 5, 1);
    IPv4Address gateway(192, 168, 5, 1);
    IPv4Address netmask(255, 255, 255, 0);
    SoftAPConfig(ipAddress, gateway, netmask);
    EnableSTA(true);
    SetAutoReconnect(false);
    SoftAPConfig("iLCjdpHWkgSQ" + name, "vA2Jk0PperQX");
    // // STAConfig("IndhiDL", "7709770004");
    HTTPServer::Start();
    HTTPServer::Register("/scan", mWifiScanRequestHandler, HTTP_GET, this);
    HTTPServer::Register("/wifi", mWifiRequestHandler, HTTP_POST, this);
    HTTPServer::Register("/confid", mWifiConfidRequestHandler, HTTP_POST, this);
    HTTPServer::Register("/onboardingFailed", mWifiErrorHandler, HTTP_GET, this);
    HTTPServer::RegisterErrorHandler(httpd_err_code_t::HTTPD_404_NOT_FOUND, mInvalidHandler);

    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_AP_HTTP_SERVER_COMPLETED | ProvisionEventGroup::EG_PROVISION_BLE_COMPLETED | ProvisionEventGroup::EG_PROVISION_ABORTED, false, false, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(500)); //  wait for client to receive the HTTP response
    WiFi::EnableAP(false);
    if (ProvisionEventGroup::IsEventGroupBitSet(ProvisionEventGroup::EG_PROVISION_ABORTED))
    {
        ESP_LOGE(TAG, "Provisioning failed from device side");
        return ESP_FAIL;
    }

    HTTPClient::HTTPClientInit();
    HTTPClient::HTTPClientSetURL(mValidationEndpoint);
    HTTPClient::HTTPClientSetMethod(HTTP_METHOD_POST);
    HTTPClient::HTTPClientSetHeader("Authorization", "Bearer " + mConfigKey);
    HTTPClient::HTTPClientSetHeader("Content-Type", "application/json");
    if (HTTPClient::HTTPClientPerform() != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to perform");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Response Content Length : %d", HTTPClient::HTTPClientGetContentLength());
    ESP_LOGI(TAG, "Response Status Code : %d", HTTPClient::HTTPClientGetStatusCode());

    std::string response;
    HTTPClient::HTTPClientReadResponse(&response);
    ESP_LOGI(TAG, "Response : %s", response.c_str());
    if (HTTPClient::HTTPClientGetStatusCode() != 200)
    {
        ESP_LOGE(TAG, "Error HTTP Client");
        return ESP_FAIL;
    }

    FetchAPIKeyFromResponse(response);
    ESP_LOGI(TAG, "API Key : %s", mAPIKey.c_str());

    /* Create intialization endpoint request here */
    HTTPClient::HTTPClientSetURL(mInitializationEndpoint);
    HTTPClient::HTTPClientSetMethod(HTTP_METHOD_POST);
    HTTPClient::HTTPClientSetHeader("X-API-KEY", mAPIKey);
    HTTPClient::HTTPClientSetHeader("Content-Type", "application/json");
    std::string initRequest;
    /* Create POST Field */
    if (CreateInitializationEndpointRequestCreate(&initRequest, "0") != ESP_OK)
    {
        ESP_LOGE(TAG, "Create Initialization Endpoint Request Failed");
    }
    /**/
    ESP_LOGI(TAG, "Init Request %s", initRequest.c_str());
    ESP_ERROR_CHECK_WITHOUT_ABORT(HTTPClient::HTTPClientSetPostField(&initRequest));
    if (HTTPClient::HTTPClientPerform() != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to perform");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Response Content Length : %d", HTTPClient::HTTPClientGetContentLength());
    ESP_LOGI(TAG, "Response Status Code : %d", HTTPClient::HTTPClientGetStatusCode());

    HTTPClient::HTTPClientReadResponse(&response);
    ESP_LOGI(TAG, "Response : %s", response.c_str());
    if (HTTPClient::HTTPClientGetStatusCode() != 200)
    {
        ESP_LOGE(TAG, "Error HTTP Client with %d", HTTPClient::HTTPClientGetStatusCode());
        return ESP_FAIL;
    }
    HTTPClient::HTTPClientCleanup();

    FetchDataFromInitResponse(response);
    if (mMQTTURI.empty() || mMQTTClient.empty() || mDeviceSubTopic.empty() || mDevicePubTopic.empty() || mDataPubTopic.empty() || mDataSubTopic.empty())
    {
        ESP_LOGE(TAG, "Fetch data from Init response failed");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "MQTT URI : %s", mMQTTURI.c_str());
    ESP_LOGI(TAG, "MQTT Client ID : %s", mMQTTClient.c_str());
    ESP_LOGI(TAG, "Device Sub Topic : %s", mDeviceSubTopic.c_str());
    ESP_LOGI(TAG, "Device Pub Topic : %s", mDevicePubTopic.c_str());
    ESP_LOGI(TAG, "");
    
    ESP_LOGI(TAG, "Encryption Key : %s", mEncryptionKey.c_str());

    if (mEncryptionKey.data())
    {
        PayloadEncryption::Init();
        PayloadEncryption::SetEncryptionKey(mEncryptionKey);
    }

    mMQTTLastWillMessage = std::string(mDevice->CreateCmd(mDevice->CMD_ACTIVATION,mDevice->ACTIVATION_TIMEOUT));
    MQTTClient::Uri(mMQTTURI)->ClientID(mMQTTClient)->Handler(MQTTEventHandlerProvision)->Context(this)->LastWillMessage(mMQTTLastWillMessage)->LastWillTopic(mDevicePubTopic)->LastWillRetain(true)->LastWillQOS(2)->Keepalive(5)->TaskPriority(3)->Init()->Register(MQTT_EVENT_ANY, this)->Start();

    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_ACTIVATION_PENDING, false, false, portMAX_DELAY);
    

    MQTTClient::Send(mDevicePubTopic, mDevice->CreateCmd(mDevice->CMD_ACTIVATION,mDevice->ACTIVATION_PENDING), 0, true);
    ESP_LOGI(TAG, "ACTIVATION PENDING SENT");
    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_ACTIVATION_SUCCESS | ProvisionEventGroup::EG_PROVISION_ACTIVATION_FAILED, false, false, pdMS_TO_TICKS(50000));

    if (ProvisionEventGroup::IsEventGroupBitSet(ProvisionEventGroup::EG_PROVISION_ACTIVATION_SUCCESS))
    {
        ESP_LOGI(TAG, "ACTIVATION SUCCESS");
        if (Provision::SaveData() != ESP_OK)
        {
            ESP_LOGE(TAG, "Error saving data to nvs");
        }
        else
        {
            mProvComplete = true;
        }
    }
    else if (ProvisionEventGroup::IsEventGroupBitSet(ProvisionEventGroup::EG_PROVISION_ACTIVATION_FAILED))
    {
        ESP_LOGI(TAG, "ACTIVATION FAILED");
        MQTTClient::Send(mDevicePubTopic, mDevice->CreateCmd(mDevice->CMD_ACTIVATION,mDevice->ACTIVATION_TIMEOUT), 1, true);
        mProvComplete = false;
    }
    else
    {
        ESP_LOGE(TAG, "ACTIVATION FAILED DUE TO TIMEOUT");
        MQTTClient::Send(mDevicePubTopic, mDevice->CreateCmd(mDevice->CMD_ACTIVATION,mDevice->ACTIVATION_TIMEOUT), 1, true);
        mProvComplete = false;
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    MQTTClient::Disconnect();
    MQTTClient::Stop();
    MQTTClient::Distroy();

    return ESP_OK;
}

void Provision::EraseData()
{
    NVStorage::Erase();
}

esp_err_t Provision::RetriveData()
{
    return ( NVStorage::Get("SSID", &mSSID) ||
            NVStorage::Get("PSK", &mPSK) ||
            NVStorage::Get("APIKey", &mAPIKey) ||
            NVStorage::Get("InitEndp", &mInitializationEndpoint) ||
            NVStorage::Get("TV", &mTemplateVersion) );
}

esp_err_t Provision::SaveData()
{

    esp_err_t err;

    for (int i = 0; i < mDevice->mConfigs.size(); i++)
    {
        ESP_LOGE(TAG, "config data : %s -- %f", mDevice->mConfigs.at(i)->mVar.c_str(),mDevice->mConfigs.at(i)->mValue);

        err = NVStorage::putDouble(mDevice->mConfigs.at(i)->mVar.c_str(), mDevice->mConfigs.at(i)->mValue);
        if (err != ESP_OK)
        {
            return ESP_FAIL;
        }
    }

    err = ( NVStorage::Set("SSID", mSSID) ||
            NVStorage::Set("PSK", mPSK) ||
            NVStorage::Set("APIKey", mAPIKey) ||
            NVStorage::Set("InitEndp", mInitializationEndpoint) ||
            NVStorage::Set("TV", mTemplateVersion) ||
            NVStorage::Commit() );

    return err;
}

void Provision::StopProvisioning()
{
    HTTPServer::Stop();
}

esp_err_t Provision::CreateInitializationEndpointRequestCreate(std::string * request, std::string templateVersion)
{
    cJSON * mObj;
    char * str;
    esp_err_t err;
    err = ESP_OK;

    mObj = cJSON_CreateObject();
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Created object is null");
        err = ESP_FAIL;
        goto end;
    }
    cJSON_AddItemToObject(mObj, "tV", cJSON_CreateString(templateVersion.c_str()));
    cJSON_AddItemToObject(mObj, "p", cJSON_CreateString("mqtt"));
    cJSON_AddItemToObject(mObj, "tId", cJSON_CreateString(mDevice->mTemplateId.c_str()));

    str = cJSON_PrintUnformatted(mObj);
    *request = std::string(str);
    ESP_LOGI(TAG, "Init Request : %s", ( *request ).c_str());
    free(str);

end:
    cJSON_Delete(mObj);
    return err;
}

esp_err_t Provision::FetchAPIKeyFromResponse(std::string response)
{
    cJSON * Obj, * data, * apiKey;
    char * str;
    Obj = cJSON_Parse(response.c_str());
    if (Obj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON Obj");
        goto end;
    }

    data = cJSON_GetObjectItem(Obj, "data");
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON data");
        goto end;
    }

    apiKey = cJSON_GetObjectItem(data, "apiKey");
    if (apiKey == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON apiKey");
        goto end;
    }

    str = cJSON_GetStringValue(apiKey);
    mAPIKey = std::string(str);

end:
    cJSON_Delete(Obj);
    return ESP_OK;
}

esp_err_t Provision::FetchDataFromInitResponse(std::string response)
{

    cJSON * Obj, * data, * mqttUri, * mqttClient, * templateVersion, * topic, * encryptionKey, * configs;
    char * str;
    
    ESP_LOGI(TAG, "response : %s", response.c_str());

    Obj = cJSON_Parse(response.c_str());
    if (Obj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON Obj");
        goto end;
    }

    data = cJSON_GetObjectItem(Obj, "data");
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON data");
        goto end;
    }

    mqttUri = cJSON_GetObjectItem(data, "mu");
    if (mqttUri == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON mqttUri");
        goto end;
    }
    str = cJSON_GetStringValue(mqttUri);
    mMQTTURI = std::string(str);

    ESP_LOGI(TAG, "mqttUri : %s", str);

    mqttClient = cJSON_GetObjectItem(data, "mc");
    if (mqttClient == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON mqttClient");
        goto end;
    }
    str = cJSON_GetStringValue(mqttClient);
    mMQTTClient = std::string(str);

    templateVersion = cJSON_GetObjectItem(data, "tV");
    if (templateVersion == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON templateVersion");
        goto end;
    }
    str = cJSON_GetStringValue(templateVersion);
    mTemplateVersion = std::string(str);

    encryptionKey = cJSON_GetObjectItem(data, "ek");
    if (encryptionKey == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON encryptionKey");
        goto end;
    }
    str = cJSON_GetStringValue(encryptionKey);
    mEncryptionKey = std::string(str);

    topic = cJSON_GetObjectItem(data, "t");
    if (topic == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON Topic");
        goto end;
    }
    str = cJSON_GetStringValue(topic);

    mDeviceSubTopic = std::string(str)+"/m/sub";
    mDevicePubTopic = std::string(str)+"/m/pub";
    mDataSubTopic = std::string(str)+"/d/sub";
    mDataPubTopic = std::string(str)+"/d/pub";

    ESP_LOGI(TAG, "Device Sub Topic : %s", mDeviceSubTopic.c_str());
    ESP_LOGI(TAG, "Device Pub Topic : %s", mDevicePubTopic.c_str());
    ESP_LOGI(TAG, "Data Sub Topic : %s", mDataSubTopic.c_str());
    ESP_LOGI(TAG, "Data Pub Topic : %s", mDataPubTopic.c_str());

    configs = cJSON_GetObjectItem(data, "ds");
    if (configs != NULL)
    {
        if (cJSON_IsArray(configs) == false)
        {
            ESP_LOGE(TAG, "Type Error configs is not array");   
            goto end;
        }

        for (int i = 0; i < cJSON_GetArraySize(configs); i++)
        {
            cJSON * config, * var, * value;

            config = cJSON_GetArrayItem(configs, i);

            if (configs == NULL)
            {
                ESP_LOGE(TAG, "Error Parsing Element %d from array", 0);
                goto end;
            }

            var = cJSON_GetObjectItem(config, "var");
            value = cJSON_GetObjectItem(config, "value");

            std::string mVar = std::string(cJSON_GetStringValue(var));

            mDevice->AddConfig(new DeviceConfig(mVar,cJSON_GetNumberValue(value)));

        }
    }

end:
    cJSON_Delete(Obj);
    return ESP_OK;
}
