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
/**
 * @brief Class MQTT client
 *
 */
class MQTTClient
{
private:
    esp_mqtt_client_config_t mClientConfig; /* MQTT Client configuration */
    esp_mqtt_client_handle_t mClientHandle; /* MQTT Client handle */
    esp_event_handler_t mEventHandler; /* Event handler */
    void * mContext; /* Context to pass to mqtt handler */
    std::string mURI; /* MQTT URI */
    std::string mHost; /* MQTT Hostname */
    uint32_t mPort; /* MQTT Port */
    std::string mClientID; /* MQTT Client ID */
    std::string mLastWillMessage; /* MQTT Last will message */
    std::string mLastWillTopic; /* MQTT Last will topic */
    int mLastWillQOS; /* MQTT Last will QOS */
    int mLastWillRetain; /* MQTT Last will retain */
    std::string mUsername; /* MQTT Username */
    std::string mPassword; /* MQTT Password */
    int mCleanSession; /* MQTT Clean session flag */
    int mKeepAlive; /* MQTT KeepAlive flag */
    bool mDisableAutoReconnect;/* MQTT Auto disable flag */

public:

    /**
     * @brief MQTT Event handler
     *
     * @param handlerArguements context to pass to the handler
     * @param base MQTT event base
     * @param id MQTT event id
     * @param eventData MQTT data
     */
    static void MQTTEventHandler(void * handlerArguements, esp_event_base_t base, int32_t id, void * eventData);
    /**
     * @brief Construct a new MQTTClient object
     *
     */
    MQTTClient();
    /**
     * @brief Construct a new MQTTClient object
     *
     * @param uri MQTT URI
     * @param clientId MQTT Client id
     * @param eventHandler MQTT event handler
     * @param context MQTT context
     */
    MQTTClient(std::string uri, std::string clientId, esp_event_handler_t eventHandler = MQTTEventHandler, void * context = nullptr);
    /**
     * @brief Set the MQTT Host
     *
     * @param host MQTT host
     * @return MQTTClient* return object
     */
    MQTTClient * Host(std::string host)
    {
        mHost = host;
        mClientConfig.host = mHost.c_str();
        return this;
    }
    /**
     * @brief Set the MQTT Client id
     *
     * @param uri MQTT URI
     * @return MQTTClient* return object
     */
    MQTTClient * Uri(std::string uri)
    {
        mURI = uri;
        mClientConfig.uri = mURI.c_str();
        return this;
    }
    /**
     * @brief Set the MQTT handler
     *
     * @param eventHandler event handler to set
     * @return MQTTClient* return object
     */
    MQTTClient * Handler(esp_event_handler_t eventHandler)
    {
        mEventHandler = eventHandler;
        return this;
    }
    /**
     * @brief Set the MQTT Port
     *
     * @param port MQTT Port
     * @return MQTTClient* return object
     */
    MQTTClient * Port(uint32_t port)
    {
        mPort = port;
        mClientConfig.port = mPort;
        return this;
    }
    /**
     * @brief Set the Client Id
     *
     * @param clientID Client id
     * @return MQTTClient* return object
     */
    MQTTClient * ClientID(std::string clientID)
    {
        mClientID = clientID;
        mClientConfig.client_id = mClientID.c_str();
        return this;
    }
    /**
     * @brief Set the MQTT Username
     *
     * @param username username
     * @return MQTTClient* return object
     */
    MQTTClient * Username(std::string username)
    {
        mUsername = username;
        mClientConfig.username = mUsername.c_str();
        return this;
    }
    /**
     * @brief Set the MQTT password
     *
     * @param password password
     * @return MQTTClient* return object
     */
    MQTTClient * Password(std::string password)
    {
        mPassword = password;
        mClientConfig.password = mPassword.c_str();
        return this;
    }
    /**
     * @brief Set the last will retain flag
     *
     * @param lastWillRetain flag
     * @return MQTTClient* return object
     */
    MQTTClient * LastWillRetain(bool lastWillRetain = true)
    {
        mLastWillRetain = lastWillRetain;
        mClientConfig.lwt_retain = mLastWillRetain;
        return this;
    }
    /**
     * @brief Set the last will QOS
     *
     * @param lastWillQos last will qos
     * @return MQTTClient*  return object
     */
    MQTTClient * LastWillQOS(int lastWillQos = 0)
    {
        mLastWillQOS = lastWillQos;
        mClientConfig.lwt_qos = mLastWillQOS;
        return this;
    }
    /**
     * @brief Set last will topic
     *
     * @param lastWillTopic
     * @return MQTTClient* return the object
     */
    MQTTClient * LastWillTopic(std::string lastWillTopic)
    {
        mLastWillTopic = lastWillTopic;
        mClientConfig.lwt_topic = mLastWillTopic.c_str();
        return this;
    }
    /**
     * @brief Set last will message
     *
     * @param lastWillMessage last will
     * @return MQTTClient* return the object
     */
    MQTTClient * LastWillMessage(std::string lastWillMessage)
    {
        mLastWillMessage = lastWillMessage;
        mClientConfig.lwt_msg = mLastWillMessage.c_str();
        mClientConfig.lwt_msg_len = mLastWillMessage.length();
        return this;
    }
    /**
     * @brief Set the last will QOS
     *
     * @param lastWillQOS
     * @return MQTTClient* return the object
     */
    MQTTClient * LWTQOS(int lastWillQOS)
    {
        mLastWillQOS = lastWillQOS;
        mClientConfig.lwt_qos = mLastWillQOS;
        return this;
    }
    /**
     * @brief Set the last will
     *
     * @param lastWillRetain
     * @return MQTTClient*
     */
    MQTTClient * LWTRetain(int lastWillRetain)
    {
        mLastWillRetain = lastWillRetain;
        mClientConfig.lwt_retain = mLastWillRetain;
        return this;
    }
    /**
     * @brief Set the MQTT Clean session flag
     *
     * @param cleanSession
     * @return MQTTClient* return the object
     */
    MQTTClient * CleanSession(int cleanSession = true)
    {
        mCleanSession = cleanSession;
        mClientConfig.disable_clean_session = mCleanSession;
        return this;
    }
    /**
     * @brief Set the keep alive flag
     *
     * @param seconds keepAlive duration
     * @return MQTTClient* return the object
     */
    MQTTClient * Keepalive(int seconds)
    {
        mKeepAlive = seconds;
        mClientConfig.keepalive = mKeepAlive;
        return this;
    }
    /**
     * @brief Set the disable auto reconnect
     *
     * @param reconnect
     * @return MQTTClient* return the object
     */
    MQTTClient * DisableAutoReconnect(bool reconnect = true)
    {
        mDisableAutoReconnect = reconnect;
        mClientConfig.disable_auto_reconnect = mDisableAutoReconnect;
        return this;
    }
    /**
     * @brief Set the MQTT Context
     *
     * @param context
     * @return MQTTClient* return the object
     */
    MQTTClient * Context(void * context)
    {
        mContext = context;
        mClientConfig.user_context = mContext;
        return this;
    }
    /**
     * @brief Set the MQTT task priority
     *
     * @param taskPriority
     * @return MQTTClient* return the object
     */
    MQTTClient * TaskPriority(int taskPriority)
    {
        mClientConfig.task_prio = taskPriority;
        return this;
    }
    /**
     * @brief Set the MQTT task size
     *
     * @param taskStackSize size of the MQTT task
     * @return MQTTClient* return the object
     */
    MQTTClient * TaskStackSize(int taskStackSize)
    {
        mClientConfig.task_stack = taskStackSize;
        return this;
    }
    /**
     * @brief Set the message buffer size
     *
     * @param bufferSize
     * @return MQTTClient*
     */
    MQTTClient * BufferSize(int bufferSize)
    {
        mClientConfig.buffer_size = bufferSize;
        return this;
    }
    /**
     * @brief Set MQTT transport
     *
     * @param transport
     * @return MQTTClient* return the object
     */
    MQTTClient * MQTTTransport(esp_mqtt_transport_t transport)
    {
        mClientConfig.transport = transport;
        return this;
    }
    /**
     * @brief Set Refresh connection
     *
     * @param milliSeconds
     * @return MQTTClient* return this object
     */
    MQTTClient * RefreshConnection(int milliSeconds)
    {
        mClientConfig.reconnect_timeout_ms = milliSeconds;
        return this;
    }
    /**
     * @brief Send the data using MQTT
     *
     * @param topic topic to send data with
     * @param data data to send
     * @param qos QOS to send with
     * @param retain retain to send with
     * @return esp_err_t success or failed
     */
    esp_err_t Send(std::string topic, std::string data, int qos = 0, int retain = 0);
    /**
     * @brief Send the data using MQTT
     *
     * @param topic topic to send data with
     * @param data data to send
     * @param qos QOS to send data with
     * @param retain retain to send with
     * @return esp_err_t success or failed
     */
    esp_err_t Send(std::string topic, std::string * data, int qos = 0, int retain = 0);
    /**
     * @brief Send the data using MQTT
     *
     * @param topic topic to send data with
     * @param data data to send
     * @param length length of the data
     * @param qos QOS to send data with
     * @param retain retain to send with
     * @return esp_err_t success or failed
     */
    esp_err_t Send(std::string topic, const char * data, int length, int qos = 0, int retain = 0);
    /**
     * @brief Initialize MQTT data
     *
     * @return MQTTClient* return this object
     */
    MQTTClient * Init();
    /**
     * @brief Reconfigure the MQTT device
     *
     */
    void ReConfig();
    /**
     * @brief Start the MQTT connection
     *
     */
    void  Start();
    /**
     * @brief Configure the MQTT connection
     *
     * @return MQTTClient* return this object
     */
    MQTTClient * Config();
    /**
     * @brief Register the MQTT handler
     *
     * @param eventId event id to of the MQTT
     * @param eventData event data structure
     * @return MQTTClient* return this object
     */
    MQTTClient * Register(esp_mqtt_event_id_t eventId, void * eventData);

    /**
     * @brief Reconnect with MQTT
     *
     */
    void Reconnect();
    /**
     * @brief Disconnect the MQTT connection
     *
     */
    void Disconnect();
    /**
     * @brief Stop the MQTT Client
     *
     */
    void Stop();
    /**
     * @brief Subscribe to MQTT topic
     *
     * @param topic topic to subscribe with
     * @param qos QOS to subscribe with
     * @return int return if subscribed or not
     */
    int Subscribe(std::string topic, int qos = 0);
    /**
     * @brief Unsubsribe from the MQTT topic
     *
     * @param topic topic to unsubscribe with
     * @return int return if subscribed or not
     */
    int UnSubscribe(std::string topic);
    /**
     * @brief Publish data to MQTT
     *
     * @param topic topic to publish with
     * @param data data to publish
     * @param qos QOS to publish with
     * @param retain retain flag to publish with
     * @return int return if published or not
     */
    int Publish(std::string topic, std::string data, int qos, int retain);
    /**
     * @brief Not used
     *
     * @param topic not used
     * @param data not used
     * @param qos not used
     * @param retain not used
     * @param store not used
     * @return int not used
     */
    int Enqueue(std::string topic, std::string data, int qos, int retain, bool store);
    /**
     * @brief Distroy the MQTT connection
     *
     */
    void Distroy();
    /**
     * @brief Not used
     *
     */
    int GetOutboxSize();
    /**
     * @brief Destroy the MQTTClient object
     *
     */
    ~MQTTClient();
};
//     } // namespace MQTTClient

// } // namespace Idf

/**
 * @brief MQTT structure to pass with event loop data
 *
 */
typedef struct
{
    char topic[100]; /* Topic to send data to */
    char data[500]; /* Data to send */
    size_t topic_len; /* Topic length */
    size_t data_len; /* Data to send */
} MQTTData;

#endif
