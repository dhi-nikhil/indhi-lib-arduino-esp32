#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "MQTTClient.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "string"
#include "string.h"

static const char * TAG = "MQTTClient";

// namespace Idf
// {
//     namespace MQTTClient
//     {

void MQTTClient::MQTTEventHandler(void * handlerArguements, esp_event_base_t base, int32_t id, void * eventData)
{
    // MQTTClient * mqtt = (MQTTClient *) handlerArgs;
    ESP_LOGI(__func__, "Event Base : %s, Event Id : %d", base, id);
}

MQTTClient::MQTTClient()
{
    memset(&mClientConfig, 0, sizeof(mClientConfig));
}

MQTTClient::MQTTClient(std::string uri, std::string clientId, esp_event_handler_t eventHandler, void * context)
{
    mURI = uri;
    mClientID = clientId;
    mEventHandler = eventHandler;
    mContext = context;

    memset(&mClientConfig, 0, sizeof(mClientConfig));
    mClientConfig.uri = mURI.c_str();
    mClientConfig.client_id = mClientID.c_str();
    mClientConfig.user_context = mContext;
}

// esp_err_t MQTTClientConfig(std::string uri, std::string clientId, )

MQTTClient * MQTTClient::Init()
{
    mClientHandle = esp_mqtt_client_init(&mClientConfig);
    if (mClientHandle == NULL)
    {
        ESP_LOGE(TAG, "Error occured");
    }
    return this;
}

esp_err_t MQTTClient::Send(std::string topic, std::string data, int qos, int retain)
{
    int messageId;
    messageId = esp_mqtt_client_publish(mClientHandle, topic.c_str(), data.c_str(), data.length(), qos, retain);
    if (messageId == -1)
    {
        ESP_LOGE(TAG, "Message publish failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Message published with id %d", messageId);
    ESP_LOGI(TAG, "Sending %s on %s topic", data.c_str(), topic.c_str());
    return ESP_OK;
}

esp_err_t MQTTClient::Send(std::string topic, std::string * data, int qos, int retain)
{
    int messageId;
    messageId = esp_mqtt_client_publish(mClientHandle, topic.c_str(), ( *data ).c_str(), ( *data ).length(), qos, retain);
    if (messageId == -1)
    {
        ESP_LOGE(TAG, "Message publish failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Message published with id %d", messageId);
    return ESP_OK;
}

esp_err_t MQTTClient::Send(std::string topic, const char * data, int length, int qos, int retain)
{
    int messageId;
    messageId = esp_mqtt_client_publish(mClientHandle, topic.c_str(), data, length, qos, retain);
    // messageId = esp_mqtt_client_publish(mClientHandle, topic.c_str(), encryptedMessage.c_str(), encryptedMessage.length() - 1, qos, retain);
    if (messageId == -1)
    {
        ESP_LOGE(TAG, "Message publish failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Message published with id %d", messageId);
    return ESP_OK;
}
// void MQTTClient::SetUri(std::string uri)
// {
//     ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_set_uri(mClientHandle, uri.c_str()));
// }

void MQTTClient::Start()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_start(mClientHandle));
}

void MQTTClient::Reconnect()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_reconnect(mClientHandle));
}

void MQTTClient::Disconnect()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_disconnect(mClientHandle));
}

void MQTTClient::Stop()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_stop(mClientHandle));
}

int MQTTClient::Subscribe(std::string topic, int qos)
{
    return ::esp_mqtt_client_subscribe(mClientHandle, topic.c_str(), qos);
}

int MQTTClient::UnSubscribe(std::string topic)
{
    return ::esp_mqtt_client_unsubscribe(mClientHandle, topic.c_str());
}

int MQTTClient::Publish(std::string topic, std::string data, int qos, int retain)
{
    return ::esp_mqtt_client_publish(mClientHandle, topic.c_str(), data.c_str(), data.length(), qos, retain);
}

int MQTTClient::Enqueue(std::string topic, std::string data, int qos, int retain, bool store)
{
    return ::esp_mqtt_client_enqueue(mClientHandle, topic.c_str(), data.c_str(), data.length(), qos, retain, store);
}

void MQTTClient::Distroy()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_destroy(mClientHandle));
}
void  MQTTClient::ReConfig()
{
    Stop();
    Config();
    Start();
}

MQTTClient * MQTTClient::Config()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_set_config(mClientHandle, &mClientConfig));
    return this;
}

MQTTClient * MQTTClient::Register(esp_mqtt_event_id_t eventId, void * eventData)
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_mqtt_client_register_event(mClientHandle, eventId, mEventHandler, eventData));
    return this;
}

int MQTTClient::GetOutboxSize()
{
    return ::esp_mqtt_client_get_outbox_size(mClientHandle);
}

MQTTClient::~MQTTClient()
{
    Distroy();
}
//     }
// }
