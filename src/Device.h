#ifndef DEVICE_H_
#define DEVICE_H_

#include "string"
#include "vector"
#include "WiFi.h"
#include "EventLoop.h"
#include "HTTPServer.h"
#include "MQTTClient.h"
#include "DefaultEventLoop.h"
#include "Task.h"
#include "Message.h"
#include "DeviceConfig.h"

/* This should be singleton */
/* This should be derived from the WiFi and others */
// class Device :public WiFi, public EventLoop

/**
 * @brief Data value callback to send the data to the callback
 *
 */
typedef esp_err_t (*dataCallback_t)( char * slot, double data, char * str);

typedef esp_err_t (*configCallback_t)( char * var, double value);

/**
 * @brief Device class to build the device
 *
 */
class Device : public NVStorage, public DefaultEventLoop, public MQTTClient
{
private:
    dataCallback_t mDataCallback; /* Data callback */
    
    configCallback_t mConfigCallback; /* config callback */

    /**
     * @brief Used to store the config
     *
     */
    std::vector<DeviceConfig *> mConfigs;
    /**
     * @brief Used to store the template Id
     *
     */
    std::string mTemplateId;
    /**
     * @brief MQTT URI to connect to MQTT
     *
     */
    std::string mMQTTURI;
    /**
     * @brief MQTT Client id to connect to MQTT
     *
     */
    std::string mMQTTClient;
    /**
     * @brief MQTT Topic for device subsription
     *
     */
    std::string mDeviceSubTopic;
    /**
     * @brief MQTT Topic for device publition
     *
     */
    std::string mDevicePubTopic;
    /**
     * @brief MQTT Topic for device data subsription
     *
     */
    std::string mDataSubTopic;
    /**
     * @brief MQTT Topic for device data publition
     *
     */
    std::string mDataPubTopic;
    /**
     * @brief Handler for device event handling
     *
     * @param handlerArguments Argument to pass with the event
     * @param base to pass with the event
     * @param id to pass with the event
     * @param eventData Event data to pass with the event
     */
    static void DeviceEventHandler(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    /**
     * @brief Handler for MQTT event
     *
     * @param handlerArguments Argument to pass with the event
     * @param base to pass with the event
     * @param id to pass with the event
     * @param eventData Event data to pass with the event
     */
    static void MQTTEventHandlerDevice(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    /**
     * @brief Reset button task
     *
     */
    Task * resetButtonTask;
public:
    enum
    {
        DEVICE_EVENT_NONE,  /* Devive event none */
        DEVICE_EVENT_DATA,  /* Device event data; not used */
        DEVICE_EVENT_INVALID,   /* Device event invalid */
        DEVICE_EVENT_DATA_RECEIVE,  /* Device event data received */
        DEVICE_EVENT_DATA_SEND, /* Device event data send */
        DEVICE_EVENT_CONNECTED, /* Device event device connected */
        DEVICE_EVENT_DISCONNECTED, /* Device event device disconnected */
        DEVICE_EVENT_MAX,
        DEVICE_SLOT_EVENT_DATA_RECEIVE,
        DEVICE_SLOT_EVENT_DATA_SEND
    };
    

    /**
     * @brief Add config to the device object
     *
     * @param Config config
     * @return DeviceMainApp* return the object if success otherwise nullptr
     */
    Device * AddConfig(DeviceConfig * config);

    double GetConfig(char * variableName);

    /**
     * @brief Construct a new Device object with name of the device
     *
     * @param templateId
     */
    Device(std::string templateId, dataCallback_t mDataCallback, configCallback_t mConfigCallback);
    void EraseData()
    {
        NVStorage::Erase();
    }
    esp_err_t Send(char * slot, double data, int decimalPlaces);
    esp_err_t Send(char * slot, char * data);

    esp_err_t SaveConfig(char * var, double data);

    std::string cJsonToSendStr(std::string key, char * data); 
    std::string cJsonToSendDouble(std::string key, double data);
    std::string cJsonToSendAck(std::string responseType, std::string data);
    std::string DecryptPayload(std::string data);
    std::string GetSlot(std::string data);
    std::string GetValueStr(std::string data);
    double GetValueDouble(std::string data);
    std::string GetMessageId(std::string data);
    Message::DataType GetDataType(std::string data);
    Message::Command::CommandType GetCommandType(std::string data);
    std::string GetURL(std::string data);
    std::string ConfigGetVar(std::string data);
    /**
     * @brief Reset button task
     *
     * @param arg
     */
    static void ResetButtonTask(void * arg);

    friend class Provision;
};

#endif
