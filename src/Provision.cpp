#define LOG_LOCAL_LEVEL ESP_LOG_NONE

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
#include "JSONParser.h"
#include "System.h"
#include "Utils.h"
#include "Device.h"
#include "string"
#include "Message.h"

#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

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

Provision::Provision(Device * device) : WiFi(), ProvisionEventGroup(), HTTPServer(), HTTPClient(), Ping(), MQTTClient(), BLEProvision()
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
            std::string data;
            data = std::string(mqttData->data, mqttData->data_len);
            ESP_LOGI(TAG, "Message %s", data.c_str());
            MessageParse * message = new MessageParse();
            message->FromString(data);
            /* Send Activation success here */
            if (message->mCommandType == Command::CommandType::COMMAND_TYPE_ACTIVATION && message->mCommandSubType == Command::CommandSubType::COMMAND_SUBTYPE_ADDITION_CONFIRMATION)
            {
                message->SetResponseType(Response::ResponseType::RESPONSE_TYPE_SUCCESS);
                provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_ACTIVATION_SUCCESS);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_FAILED);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_PENDING);
            }
            else
            {
                message->SetResponseType(Response::ResponseType::RESPONSE_TYPE_FAILED);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_SUCCESS);
                provision->ProvisionEventGroup::EventGroupSetBits(Provision::EG_PROVISION_ACTIVATION_FAILED);
                provision->ProvisionEventGroup::EventGroupClearBits(Provision::EG_PROVISION_ACTIVATION_PENDING);
            }
            provision->MQTTClient::Send(provision->mDevicePubTopic, message->ToString(), 0, true);
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
    if (CreateInitializationEndpointRequestCreate(&initRequest) != ESP_OK)
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
    ESP_LOGI(TAG, "MQTT Connection Verification : %d", mMQTTConnectionVerification);

    Message::EncryptionEnable(false);
    if (!mEncryptionKey.empty())
    {
        PayloadEncryption::Init();
        Message::EncryptionEnable(true);
        PayloadEncryption::SetEncryptionKey(mEncryptionKey);
    }
    mDevice->mMQTTURI = mMQTTURI;
    mDevice->mMQTTClient = mMQTTClient;
    mDevice->mDevicePubTopic = mDevicePubTopic;
    mDevice->mDeviceSubTopic = mDeviceSubTopic;
    mDevice->mDataPubTopic = mDataPubTopic;
    mDevice->mDataSubTopic = mDataSubTopic;

    MessageCreate * message = new MessageCreate();
    message->SetCommandType(Message::CommandType::COMMAND_TYPE_DISCONNECTED);
    mMQTTLastWillMessage = std::string(message->ToString());
    ESP_LOGI(TAG, "Disconnected message : %s", mMQTTLastWillMessage.c_str());
    mDevice->MQTTClient::Uri(mMQTTURI)->ClientID(mMQTTClient)->Handler(mDevice->MQTTEventHandlerDevice)->Context(mDevice)->LastWillMessage(mMQTTLastWillMessage)->LastWillTopic(mDevicePubTopic)->LastWillRetain(true)->LastWillQOS(2)->Keepalive(5)->Init()->Register(MQTT_EVENT_ANY, mDevice)->Start();
    return ESP_OK;
}

void Provision::GAPEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t * param)
{
    ESP_LOGI(TAG, "GAP event : %d", event);
    switch (event)
    {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            BLEProvision::StartAdvertising();
            break;
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            BLEProvision::StartAdvertising();
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            break;
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            break;
        case ESP_GAP_BLE_EVT_MAX:
            break;
        default:
            break;
    }
}

void Provision::GATTEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t * param)
{
    ESP_LOGI(TAG, "GATT event : %d", event);

    esp_err_t ErrorCode = ESP_OK;
    switch (event)
    {
        case ESP_GATTS_REG_EVT:
        {
            BLEProvision::BLEGAPConfigure();
            BLEProvision::ble_onboarding_profile.gatts_if = gatts_if;
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gatts_create_service(gatts_if, &ble_onboarding_profile.wifi_scan->service_id, ble_onboarding_profile.wifi_scan->service_num_handle));
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gatts_create_service(gatts_if, &ble_onboarding_profile.wifi_config->service_id, ble_onboarding_profile.wifi_config->service_num_handle));
            ESP_LOGI(TAG, "app_id : %04x, status : %d", param->reg.app_id, param->reg.status);
        }
        break;
        case ESP_GATTS_CREATE_EVT:
        {
            if (param->create.service_id.id.uuid.uuid.uuid16 == BLEProvision::WIFI_SCAN_SERVICE_UUID)
            {
                ble_onboarding_profile.wifi_scan->service_handle = param->create.service_handle;
                ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gatts_start_service(ble_onboarding_profile.wifi_scan->service_handle));
                for (int i = 0; i < ble_onboarding_profile.wifi_scan->charas_num; i++)
                {
                    ESP_LOGI(TAG, "I : %d", i);
                    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_gatts_add_char(ble_onboarding_profile.wifi_scan->service_handle, &ble_onboarding_profile.wifi_scan->charas[i]->char_uuid, ble_onboarding_profile.wifi_scan->charas[i]->perm, ble_onboarding_profile.wifi_scan->charas[i]->property, &ble_onboarding_profile.wifi_scan->charas[i]->val, ESP_GATT_RSP_BY_APP));
                }
            }
            else if (param->create.service_id.id.uuid.uuid.uuid16 == BLEProvision::WIFI_CONF_SERVICE_UUID)
            {
                ble_onboarding_profile.wifi_config->service_handle = param->create.service_handle;
                esp_ble_gatts_start_service(ble_onboarding_profile.wifi_config->service_handle);
                for (int i = 0; i < ble_onboarding_profile.wifi_config->charas_num; i++)
                {
                    esp_ble_gatts_add_char(ble_onboarding_profile.wifi_config->service_handle, &ble_onboarding_profile.wifi_config->charas[i]->char_uuid, ble_onboarding_profile.wifi_config->charas[i]->perm, ble_onboarding_profile.wifi_config->charas[i]->property, &ble_onboarding_profile.wifi_config->charas[i]->val, ESP_GATT_RSP_BY_APP);
                }
            }
            else
            {
            }
        }
        break;
        case ESP_GATTS_ADD_CHAR_EVT:
        {
            if (param->add_char.service_handle == ble_onboarding_profile.wifi_scan->service_handle)
            {
                for (int i = 0; i < ble_onboarding_profile.wifi_scan->charas_num; i++)
                {
                    if (param->add_char.char_uuid.len == ble_onboarding_profile.wifi_scan->charas[i]->char_uuid.len && param->add_char.char_uuid.uuid.uuid16 == ble_onboarding_profile.wifi_scan->charas[i]->char_uuid.uuid.uuid16)
                    {
                        ble_onboarding_profile.wifi_scan->charas[i]->char_handle = param->add_char.attr_handle;
                    }
                }
            }
            else if (param->add_char.service_handle == ble_onboarding_profile.wifi_config->service_handle)
            {
                for (int i = 0; i < ble_onboarding_profile.wifi_config->charas_num; i++)
                {
                    if (param->add_char.char_uuid.len == ble_onboarding_profile.wifi_config->charas[i]->char_uuid.len && param->add_char.char_uuid.uuid.uuid16 == ble_onboarding_profile.wifi_config->charas[i]->char_uuid.uuid.uuid16)
                    {
                        ble_onboarding_profile.wifi_config->charas[i]->char_handle = param->add_char.attr_handle;
                    }
                }
            }
            else
            {
            }
        }
        break;
        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        {
            if (param->add_char_descr.service_handle == ble_onboarding_profile.wifi_scan->service_handle)
            {
                for (int i = 0; i < ble_onboarding_profile.wifi_scan->charas_num; i++)
                {
                    if (param->add_char_descr.descr_uuid.len == ble_onboarding_profile.wifi_scan->charas[i]->descr_uuid.len && param->add_char_descr.descr_uuid.uuid.uuid16 == ble_onboarding_profile.wifi_scan->charas[i]->descr_uuid.uuid.uuid16)
                    {
                        ble_onboarding_profile.wifi_scan->charas[i]->descr_handle = param->add_char_descr.attr_handle;
                    }
                }
            }
            else if (param->add_char_descr.service_handle == ble_onboarding_profile.wifi_config->service_handle)
            {
                for (int i = 0; i < ble_onboarding_profile.wifi_config->charas_num; i++)
                {
                    if (param->add_char_descr.descr_uuid.len == ble_onboarding_profile.wifi_config->charas[i]->descr_uuid.len && param->add_char_descr.descr_uuid.uuid.uuid16 == ble_onboarding_profile.wifi_config->charas[i]->descr_uuid.uuid.uuid16)
                    {
                        ble_onboarding_profile.wifi_config->charas[i]->descr_handle = param->add_char_descr.attr_handle;
                    }
                }
            }
            else
            {
            }
        }
        break;
        case ESP_GATTS_READ_EVT:
        {
            esp_gatt_rsp_t * esp_gatt_rsp = (esp_gatt_rsp_t *) calloc(1, sizeof(esp_gatt_rsp_t));
            if (param->read.handle == ble_onboarding_profile.wifi_scan->charas[0]->char_handle)
            {
                esp_gatt_rsp->attr_value.len = 1;
                esp_gatt_rsp->attr_value.value[0] = 6;
                provision->WiFiScanEventGroup::EventGroupGetBits();
                provision->WiFiSTAEventGroup::EventGroupGetBits();
                if (provision->WiFiScanEventGroup::IsEventGroupBitSet(EG_SCAN_IDLE))
                {
                    esp_gatt_rsp->attr_value.value[0] = 0;
                }

                if (provision->WiFiScanEventGroup::IsEventGroupBitSet(EG_SCAN_COMPLETED))
                {
                    esp_gatt_rsp->attr_value.value[0] = 3;
                }
                else if (provision->WiFiScanEventGroup::IsEventGroupBitSet(EG_SCAN_INPROCESS))
                {
                    esp_gatt_rsp->attr_value.value[0] = 2;
                }

                if (provision->WiFiSTAEventGroup::IsEventGroupBitSet(EG_STA_CONNECTED))
                {
                    esp_gatt_rsp->attr_value.value[0] = 5;
                }
                else if (provision->WiFiSTAEventGroup::IsEventGroupBitSet(EG_STA_DISCONNECT))
                {
                    esp_gatt_rsp->attr_value.value[0] = 7;
                }
                else if (provision->WiFiSTAEventGroup::IsEventGroupBitSet(EG_STA_CONNECTING))
                {
                    esp_gatt_rsp->attr_value.value[0] = 8;
                }
                ESP_LOGI(TAG, "WIFI SCAN : %d", esp_gatt_rsp->attr_value.value[0]);
            }
            else if (param->read.handle == ble_onboarding_profile.wifi_scan->charas[1]->char_handle)
            {
                static char * scan_result = nullptr;
                static uint16_t scanEntries = 0;

                esp_gatt_rsp->attr_value.handle = param->read.handle;
                esp_gatt_rsp->attr_value.offset = param->read.offset;
                if (param->read.is_long == true)
                {
                    ESP_LOGI(TAG, "Sending response from fetch results");
                    esp_gatt_rsp->attr_value.len = strlen(scan_result) - param->read.offset;
                }
                else
                {
                    ESP_LOGI(TAG, "Fetching new scan result");
                    scanEntries = provision->GetScanEntries();
                    cJSON * arr;
                    arr = cJSON_CreateArray();
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

                    scan_result = cJSON_PrintUnformatted(arr);
                    ESP_LOG_BUFFER_HEX(__func__, scan_result, strlen(scan_result));
                    ESP_LOGI(TAG, "data : %s, len : %d", scan_result, strlen(scan_result));
                    esp_gatt_rsp->attr_value.len = strlen(scan_result);
                }
                memcpy(esp_gatt_rsp->attr_value.value, &scan_result[param->read.offset], esp_gatt_rsp->attr_value.len);
            }
            else if (param->read.handle == ble_onboarding_profile.wifi_config->charas[0]->char_handle)
            {
                esp_gatt_rsp->attr_value.len = 1;
                provision->PingEventGroup::EventGroupGetBits();
                if (provision->PingEventGroup::IsEventGroupBitSet(EG_PING_SUCCESS))
                {
                    esp_gatt_rsp->attr_value.value[0] = 2;
                }
                else if (provision->PingEventGroup::IsEventGroupBitSet(EG_PING_PROCESSED))
                {
                    esp_gatt_rsp->attr_value.value[0] = 1;
                }
                else if (provision->PingEventGroup::IsEventGroupBitSet(EG_PING_PROCESSING))
                {
                    esp_gatt_rsp->attr_value.value[0] = 4;
                }
                else if (provision->PingEventGroup::IsEventGroupBitSet(EG_PING_IDLE))
                {
                    esp_gatt_rsp->attr_value.value[0] = 0;
                }
                else
                {
                    esp_gatt_rsp->attr_value.value[0] = 3;
                }

                if (esp_gatt_rsp->attr_value.value[0] == 2)
                {
                    provision->ProvisionEventGroup::EventGroupSetBits(ProvisionEventGroup::EG_PROVISION_AP_HTTP_SERVER_COMPLETED);
                }

                ESP_LOGI(TAG, "WIFI CONFIG : %d", esp_gatt_rsp->attr_value.value[0]);
            }
            else
            {
            }

            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, esp_gatt_rsp);
            free(esp_gatt_rsp);
        }
        break;
        case ESP_GATTS_WRITE_EVT:
        {
            ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);

            esp_gatt_rsp_t * esp_gatt_rsp = (esp_gatt_rsp_t *) calloc(1, sizeof(esp_gatt_rsp_t));
            if (param->write.handle == ble_onboarding_profile.wifi_scan->charas[0]->char_handle)
            {
                if (param->write.value[0] == WIFI_SCANNER_STATE_SCAN)
                {
                    WiFiScanStatus wifiScanStatus = provision->Scan(false);
                }
            }
            else if (param->write.handle == ble_onboarding_profile.wifi_config->charas[1]->char_handle)
            {
                if (param->write.need_rsp)
                {
                    ESP_LOGI(TAG, "needs response");
                    if (param->write.is_prep)
                    {
                        ESP_LOGI(TAG, "is prep");
                    }
                    else
                    {
                        std::string config_write;
                        std::string initializationEndpoint;
                        std::string validationEndpoint;
                        std::string configKey;
                        std::string ipAddress;

                        config_write = std::string((char *) param->write.value, param->write.len);
                        ESP_LOGI(TAG, "CONFIG other : %s", config_write.c_str());

                        ErrorCode = ( JSONParser::JSONParserParseValueByKey(&validationEndpoint, "device_validation_endpoint", config_write) ||
                                     JSONParser::JSONParserParseValueByKey(&initializationEndpoint, "device_initialization_endpoint", config_write) ||
                                     JSONParser::JSONParserParseValueByKey(&configKey, "validation_token", config_write) ||
                                     JSONParser::JSONParserParseValueByKey(&ipAddress, "server_ip_address", config_write) );

                        if (ErrorCode != ESP_OK)
                        {
                            ESP_LOGI(TAG, "error while parsing ");
                        }
                        provision->PingIPv4Address(ipAddress, 0);

                        provision->mValidationEndpoint = validationEndpoint;
                        provision->mInitializationEndpoint = initializationEndpoint;
                        provision->mConfigKey = configKey;
                        provision->mIPAddress = ipAddress;
                    }
                }
            }
            else if (param->write.handle == ble_onboarding_profile.wifi_scan->charas[2]->char_handle)
            {
                if (param->write.need_rsp)
                {
                    ESP_LOGI(TAG, "needs response");
                    if (param->write.is_prep)
                    {
                        ESP_LOGI(TAG, "is prep");
                    }
                    else
                    {
                        std::string config_write;
                        std::string sta_ssid;
                        std::string sta_password;

                        config_write = std::string((char *) param->write.value, param->write.len);
                        ESP_LOGI(TAG, "CONFIG SSID PASSWORD : %s", config_write.c_str());

                        ErrorCode = ( JSONParser::JSONParserParseValueByKey(&sta_ssid, "ssid", config_write) ||
                                     JSONParser::JSONParserParseValueByKey(&sta_password, "psk", config_write) );

                        if (ErrorCode != ESP_OK)
                        {
                            ESP_LOGI(TAG, "error while parsing ");
                        }
                        provision->WiFiStation::Disconnect();
                        provision->STAConfig(sta_ssid, sta_password, 0, nullptr, true);
                        provision->WiFiSTAEventGroup::EventGroupSetBits(EG_STA_CONNECTING);
                        provision->mSSID = sta_ssid;
                        provision->mPSK = sta_password;
                        ESP_LOGI(TAG, "SSID : %s", sta_ssid.c_str());
                        ESP_LOGI(TAG, "PSK : %s", sta_password.c_str());
                    }
                }
            }

            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, esp_gatt_rsp);
            free(esp_gatt_rsp);
        }
        break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            break;
        case ESP_GATTS_START_EVT:
            break;
        case ESP_GATTS_CONNECT_EVT:
            BLEProvision::StopAdvertising();
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            BLEProvision::StartAdvertising();
            break;
        case ESP_GATTS_RESPONSE_EVT:
            ESP_LOGI(TAG, "handle : %d; status : %d", param->rsp.handle, param->rsp.status);
            break;
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
            break;
        default:
            break;
    }
}

esp_err_t Provision::StartProvisioningBLE()
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

    WiFi::Start();
    WiFi::EnableSTA(true);
    SetAutoReconnect(false);

    /* BLE */
    BLEProvision::Init();
    BLEProvision::SetName(mDevice->mName);
    BLEProvision::SetGAPCallback(Provision::GAPEventHandler);
    BLEProvision::SetGATTCallback(Provision::GATTEventHandler);
    BLEProvision::ConfigureProvisionGATT();
    BLEProvision::Start();

    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_AP_HTTP_SERVER_COMPLETED | ProvisionEventGroup::EG_PROVISION_BLE_COMPLETED | ProvisionEventGroup::EG_PROVISION_ABORTED, false, false, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(1000)); //  wait for client to receive the HTTP response
    BLEProvision::Stop();
    BLEProvision::Deinit();

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
    if (CreateInitializationEndpointRequestCreate(&initRequest) != ESP_OK)
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
    ESP_LOGI(TAG, "MQTT Connection Verification : %d", mMQTTConnectionVerification);

    Message::EncryptionEnable(false);
    if (mEncryptionKey.data())
    {
        PayloadEncryption::Init();
        Message::EncryptionEnable(true);
        PayloadEncryption::SetEncryptionKey(mEncryptionKey);
    }

    MessageCreate * message = new MessageCreate();
    message->SetCommandType(Message::CommandType::COMMAND_TYPE_ACTIVATION);
    message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
    mMQTTLastWillMessage = std::string(message->ToString());
    MQTTClient::Uri(mMQTTURI)->ClientID(mMQTTClient)->Handler(MQTTEventHandlerProvision)->Context(this)->LastWillMessage(mMQTTLastWillMessage)->LastWillTopic(mDevicePubTopic)->LastWillRetain(true)->LastWillQOS(2)->Keepalive(5)->Init()->Register(MQTT_EVENT_ANY, this)->Start();

    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_ACTIVATION_PENDING, false, false, portMAX_DELAY);
    message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_PENDING);
    MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
    ESP_LOGI(TAG, "ACTIVATION PENDING SENT");
    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_ACTIVATION_SUCCESS | ProvisionEventGroup::EG_PROVISION_ACTIVATION_FAILED, false, false, pdMS_TO_TICKS(50000));

    if (ProvisionEventGroup::IsEventGroupBitSet(ProvisionEventGroup::EG_PROVISION_ACTIVATION_SUCCESS))
    {
        ESP_LOGI(TAG, "ACTIVATION SUCCESS");
        if (Provision::SaveData() != ESP_OK)
        {
            ESP_LOGE(TAG, "Error saving data to nvs");
        }
        mProvComplete = true;
    }
    else if (ProvisionEventGroup::IsEventGroupBitSet(ProvisionEventGroup::EG_PROVISION_ACTIVATION_FAILED))
    {
        ESP_LOGI(TAG, "ACTIVATION FAILED");
        message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
        MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
        mProvComplete = false;
    }
    else
    {
        ESP_LOGE(TAG, "ACTIVATION FAILED DUE TO TIMEOUT");
        message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
        MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
        mProvComplete = false;
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    MQTTClient::Disconnect();
    MQTTClient::Stop();
    MQTTClient::Distroy();

    return ESP_OK;
}

esp_err_t Provision::StartProvisioningWiFi()
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
    SoftAPConfig("iLCjdpHWkgSQ" + mDevice->mName, "vA2Jk0PperQX");
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
    if (CreateInitializationEndpointRequestCreate(&initRequest) != ESP_OK)
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
    ESP_LOGI(TAG, "MQTT Connection Verification : %d", mMQTTConnectionVerification);

    Message::EncryptionEnable(false);
    if (mEncryptionKey.data())
    {
        PayloadEncryption::Init();
        Message::EncryptionEnable(true);
        PayloadEncryption::SetEncryptionKey(mEncryptionKey);
    }

    MessageCreate * message = new MessageCreate();
    message->SetCommandType(Message::CommandType::COMMAND_TYPE_ACTIVATION);
    message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
    mMQTTLastWillMessage = std::string(message->ToString());
    MQTTClient::Uri(mMQTTURI)->ClientID(mMQTTClient)->Handler(MQTTEventHandlerProvision)->Context(this)->LastWillMessage(mMQTTLastWillMessage)->LastWillTopic(mDevicePubTopic)->LastWillRetain(true)->LastWillQOS(2)->Keepalive(5)->Init()->Register(MQTT_EVENT_ANY, this)->Start();

    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_ACTIVATION_PENDING, false, false, portMAX_DELAY);
    message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_PENDING);
    MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
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
        message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
        MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
        mProvComplete = false;
    }
    else
    {
        ESP_LOGE(TAG, "ACTIVATION FAILED DUE TO TIMEOUT");
        message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
        MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
        mProvComplete = false;
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    MQTTClient::Disconnect();
    MQTTClient::Stop();
    MQTTClient::Distroy();

    return ESP_OK;
}

/* For token based onboarding */
esp_err_t Provision::StartProvisioningWiFi(std::string ssid, std::string psk, std::string token)
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

    if (ssid.empty() || psk.empty() || token.empty())
    {
        ESP_LOGE(TAG, "One or many fields are empty");
        return ESP_FAIL;
    }

    Provision::SetConfigKey(token);
    mSSID = ssid;
    mPSK = psk;

    EnableSTA(true);
    STAConfig(mSSID, mPSK);
    SetAutoReconnect(false);

    WiFiSTAEventGroup::EventGroupWaitBits(WiFiSTAEventGroup::EG_STA_CONNECTED, false, false, pdMS_TO_TICKS(20000));
    vTaskDelay(pdMS_TO_TICKS(5000));
    if (WiFiSTAEventGroup::IsEventGroupBitSet(WiFiSTAEventGroup::EG_STA_CONNECTED))
    {
        ESP_LOGI(TAG, "STA connection success");
    }
    else
    {
        ESP_LOGE(TAG, "STA connection failed");
        return ESP_FAIL;
    }
    // ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_AP_HTTP_SERVER_COMPLETED | EG_PROVISION_BLE_COMPLETED, false, false, portMAX_DELAY);

    HTTPClient::HTTPClientInit();
    HTTPClient::HTTPClientSetURL(mValidationEndpoint);
    HTTPClient::HTTPClientSetMethod(HTTP_METHOD_POST);
    HTTPClient::HTTPClientSetHeader("Authorization", "Bearer " + mConfigKey);
    HTTPClient::HTTPClientSetHeader("Content-Type", "application/json");
    if (HTTPClient::HTTPClientPerform() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to perform");
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
    if (CreateInitializationEndpointRequestCreate(&initRequest) != ESP_OK)
    {
        ESP_LOGE(TAG, "Create Initialization Endpoint Request Failed");
        return ESP_FAIL;
    }
    /**/
    ESP_LOGI(TAG, "Init Request %s", initRequest.c_str());
    ESP_ERROR_CHECK_WITHOUT_ABORT(HTTPClient::HTTPClientSetPostField(&initRequest));
    if (HTTPClient::HTTPClientPerform() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to perform");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Response Content Length : %d", HTTPClient::HTTPClientGetContentLength());
    ESP_LOGI(TAG, "Response Status Code : %d", HTTPClient::HTTPClientGetStatusCode());

    HTTPClient::HTTPClientReadResponse(&response);
    ESP_LOGI(TAG, "Response : %s", response.c_str());
    if (HTTPClient::HTTPClientGetStatusCode() != 200)
    {
        ESP_LOGE(TAG, "Error HTTP Client");
        return ESP_FAIL;
    }
    HTTPClient::HTTPClientCleanup();

    /* Fetch Response data here */
    /* MQTT URI, CLIENT ID, DEVICE SUB TOPIC, DEVICE PUB TOPIC */
    /* FEATURE SUB TOPIC,FEATURE PUB TOPIC, VERSION CODE, IDENTIFIER */
    /* MCV REQUIRED, EDKEY */

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
    ESP_LOGI(TAG, "MQTT Connection Verification : %d", mMQTTConnectionVerification);

    /* Start the Provisioning MQTT Client */
    /* if mcv is true handle the mqtt events here */
    /* if not handle the mqtt events in the device */
    Message::EncryptionEnable(false);
    if (mEncryptionKey.data())
    {
        PayloadEncryption::Init();
        Message::EncryptionEnable(true);
        PayloadEncryption::SetEncryptionKey(mEncryptionKey);
    }

    MessageCreate * message = new MessageCreate();
    message->SetCommandType(Message::CommandType::COMMAND_TYPE_ACTIVATION);
    message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
    mMQTTLastWillMessage = std::string(message->ToString());
    MQTTClient::Uri(mMQTTURI)->ClientID(mMQTTClient)->Handler(MQTTEventHandlerProvision)->Context(this)->LastWillMessage(mMQTTLastWillMessage)->LastWillTopic(mDevicePubTopic)->LastWillRetain(true)->LastWillQOS(2)->Keepalive(5)->Init()->Register(MQTT_EVENT_ANY, this)->Start();

    ProvisionEventGroup::EventGroupWaitBits(ProvisionEventGroup::EG_PROVISION_ACTIVATION_PENDING, false, false, portMAX_DELAY);
    message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_PENDING);
    MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
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
        message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
        MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
        mProvComplete = false;
    }
    else
    {
        ESP_LOGE(TAG, "ACTIVATION FAILED DUE TO TIMEOUT");
        message->SetStatusType(Message::StatusType::STATUS_TYPE_ACTIVATION_TIMEOUT);
        MQTTClient::Send(mDevicePubTopic, message->ToString(), 0, true);
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
    NVStorage::Erase("SSID");
    NVStorage::Erase("PSK");
    NVStorage::Erase("APIKey");
    NVStorage::Erase("InitEndp");
}

esp_err_t Provision::RetriveData()
{
    return ( NVStorage::Get("SSID", &mSSID) ||
            NVStorage::Get("PSK", &mPSK) ||
            NVStorage::Get("APIKey", &mAPIKey) ||
            NVStorage::Get("InitEndp", &mInitializationEndpoint) );
}

esp_err_t Provision::SaveData()
{
    return ( NVStorage::Set("SSID", mSSID) ||
            NVStorage::Set("PSK", mPSK) ||
            NVStorage::Set("APIKey", mAPIKey) ||
            NVStorage::Set("InitEndp", mInitializationEndpoint) ||
            NVStorage::Commit() );
}

void Provision::StopProvisioning()
{
    HTTPServer::Stop();
}

esp_err_t Provision::CreateInitializationEndpointRequestCreate(std::string * request)
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
    cJSON_AddItemToObject(mObj, "version", cJSON_CreateString("2"));
    cJSON_AddItemToObject(mObj, "deviceName", cJSON_CreateString(mDevice->mName.c_str()));
    cJSON_AddItemToObject(mObj, "templateId", cJSON_CreateString(mDevice->mTemplateId.c_str()));

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

/* TODO : VERIFY THE FUNCTION */
esp_err_t Provision::FetchDataFromInitResponse(std::string response)
{
    cJSON * Obj, * data, * mqttUri, * mqttClient, * deviceSubTopic, * devicePubTopic, * features, * encryptionKey, * mqttConnectionVerification;
    char * str;
    int num;

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

    mqttUri = cJSON_GetObjectItem(data, "mqttUri");
    if (mqttUri == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON mqttUri");
        goto end;
    }
    str = cJSON_GetStringValue(mqttUri);
    mMQTTURI = std::string(str);

    mqttClient = cJSON_GetObjectItem(data, "mqttClient");
    if (mqttClient == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON mqttClient");
        goto end;
    }
    str = cJSON_GetStringValue(mqttClient);
    mMQTTClient = std::string(str);

    deviceSubTopic = cJSON_GetObjectItem(data, "deviceSubTopic");
    if (deviceSubTopic == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON deviceSubTopic");
        goto end;
    }
    str = cJSON_GetStringValue(deviceSubTopic);
    mDeviceSubTopic = std::string(str);

    devicePubTopic = cJSON_GetObjectItem(data, "devicePubTopic");
    if (devicePubTopic == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON devicePubTopic");
        goto end;
    }
    str = cJSON_GetStringValue(devicePubTopic);
    mDevicePubTopic = std::string(str);

    mqttConnectionVerification = cJSON_GetObjectItem(data, "mcvRequired");
    if (mqttConnectionVerification == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON mqttConnectionVerification");
        goto end;
    }
    num = cJSON_GetNumberValue(mqttConnectionVerification);
    mMQTTConnectionVerification = num;

    encryptionKey = cJSON_GetObjectItem(data, "edKey");
    if (encryptionKey == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing JSON encryptionKey");
        goto end;
    }
    str = cJSON_GetStringValue(encryptionKey);
    mEncryptionKey = std::string(str);

    cJSON * pubTopic, * subTopic;

    pubTopic = cJSON_GetObjectItem(data, "featurePubTopic");
    subTopic = cJSON_GetObjectItem(data, "featureSubTopic");
    
    str = cJSON_GetStringValue(subTopic);
    mDataSubTopic = std::string(str);
    
    str = cJSON_GetStringValue(pubTopic);
    mDataPubTopic = std::string(str);

end:
    cJSON_Delete(Obj);
    return ESP_OK;
}
