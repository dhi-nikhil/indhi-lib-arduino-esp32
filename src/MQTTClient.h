#ifndef MQTTCLIENT_H_
#define MQTTCLIENT_H_

#include "freertos/FreeRTOS.h"
#include "mqtt_client.h"
#include "esp_event_base.h"
#include "esp_event.h"
#include "string"
#include "PayloadEncryption.h"
#include "esp_log.h"

// namespace Idf
// {
//     namespace MQTTClient
//     {
class MQTTClient
{
private:
    esp_mqtt_client_config_t mClientConfig;
    esp_mqtt_client_handle_t mClientHandle;
    esp_event_handler_t mEventHandler;
    void * mContext;
    std::string mURI;
    std::string mHost;
    uint32_t mPort;
    std::string mClientID;
    std::string mLastWillMessage;
    std::string mLastWillTopic;
    int mLastWillQOS;
    int mLastWillRetain;
    std::string mUsername;
    std::string mPassword;
    int mCleanSession;
    int mKeepAlive;
    bool mDisableAutoReconnect;

public:

    static void MQTTEventHandler(void * handlerArguements, esp_event_base_t base, int32_t id, void * eventData);
    MQTTClient();
    MQTTClient(std::string uri, std::string clientId, esp_event_handler_t eventHandler = MQTTEventHandler, void * context = nullptr);
    MQTTClient * Host(std::string host)
    {
        mHost = host;
        mClientConfig.host = mHost.c_str();
        return this;
    }
    MQTTClient * Uri(std::string uri)
    {
        mURI = uri;
        mClientConfig.uri = mURI.c_str();
        return this;
    }
    MQTTClient * Handler(esp_event_handler_t eventHandler)
    {
        mEventHandler = eventHandler;
        return this;
    }
    MQTTClient * Port(uint32_t port)
    {
        mPort = port;
        mClientConfig.port = mPort;
        return this;
    }
    MQTTClient * ClientID(std::string clientID)
    {
        mClientID = clientID;
        mClientConfig.client_id = mClientID.c_str();
        return this;
    }
    MQTTClient * Username(std::string username)
    {
        mUsername = username;
        mClientConfig.username = mUsername.c_str();
        return this;
    }
    MQTTClient * Password(std::string password)
    {
        mPassword = password;
        mClientConfig.password = mPassword.c_str();
        return this;
    }
    MQTTClient * LastWillRetain(bool lastWillRetain = true)
    {
        mLastWillRetain = lastWillRetain;
        mClientConfig.lwt_retain = mLastWillRetain;
        return this;
    }
    MQTTClient * LastWillQOS(int lastWillQos = 0)
    {
        mLastWillQOS = lastWillQos;
        mClientConfig.lwt_qos = mLastWillQOS;
        return this;
    }
    MQTTClient * LastWillTopic(std::string lastWillTopic)
    {
        mLastWillTopic = lastWillTopic;
        mClientConfig.lwt_topic = mLastWillTopic.c_str();
        return this;
    }
    MQTTClient * LastWillMessage(std::string lastWillMessage)
    {
        mLastWillMessage = lastWillMessage;
        mClientConfig.lwt_msg = mLastWillMessage.c_str();
        mClientConfig.lwt_msg_len = mLastWillMessage.length();
        return this;
    }
    MQTTClient * LWTQOS(int lastWillQOS)
    {
        mLastWillQOS = lastWillQOS;
        mClientConfig.lwt_qos = mLastWillQOS;
        return this;
    }
    MQTTClient * LWTRetain(int lastWillRetain)
    {
        mLastWillRetain = lastWillRetain;
        mClientConfig.lwt_retain = mLastWillRetain;
        return this;
    }
    MQTTClient * CleanSession(int cleanSession = true)
    {
        mCleanSession = cleanSession;
        mClientConfig.disable_clean_session = mCleanSession;
        return this;
    }
    MQTTClient * Keepalive(int seconds)
    {
        mKeepAlive = seconds;
        mClientConfig.keepalive = mKeepAlive;
        return this;
    }
    MQTTClient * DisableAutoReconnect(bool reconnect = true)
    {
        mDisableAutoReconnect = reconnect;
        mClientConfig.disable_auto_reconnect = mDisableAutoReconnect;
        return this;
    }
    MQTTClient * Context(void * context)
    {
        mContext = context;
        mClientConfig.user_context = mContext;
        return this;
    }
    MQTTClient * TaskPriority(int taskPriority)
    {
        mClientConfig.task_prio = taskPriority;
        return this;
    }
    MQTTClient * TaskStackSize(int taskStackSize)
    {
        mClientConfig.task_stack = taskStackSize;
        return this;
    }
    MQTTClient * BufferSize(int bufferSize)
    {
        mClientConfig.buffer_size = bufferSize;
        return this;
    }
    MQTTClient * MQTTTransport(esp_mqtt_transport_t transport)
    {
        mClientConfig.transport = transport;
        return this;
    }
    MQTTClient * RefreshConnection(int milliSeconds)
    {
        mClientConfig.reconnect_timeout_ms = milliSeconds;
        return this;
    }
    esp_err_t Send(std::string topic, std::string data, int qos = 0, int retain = 0);
    esp_err_t Send(std::string topic, std::string * data, int qos = 0, int retain = 0);
    esp_err_t Send(std::string topic, const char * data, int length, int qos = 0, int retain = 0);
    MQTTClient * Init();
    void ReConfig();
    void  Start();
    MQTTClient * Config();
    MQTTClient * Register(esp_mqtt_event_id_t eventId, void * eventData);

    void Reconnect();
    void Disconnect();
    void Stop();
    int Subscribe(std::string topic, int qos = 0);
    int UnSubscribe(std::string topic);
    int Publish(std::string topic, std::string data, int qos, int retain);
    int Enqueue(std::string topic, std::string data, int qos, int retain, bool store);
    void Distroy();
    int GetOutboxSize();
    ~MQTTClient();
};
//     } // namespace MQTTClient

// } // namespace Idf

typedef struct
{
    char topic[100];
    char data[500];
    size_t topic_len;
    size_t data_len;
} MQTTData;

#endif
