#ifndef DEVICE_H_
#define DEVICE_H_

#include "Feature.h"
#include "string"
#include "vector"
#include "WiFi.h"
#include "EventLoop.h"
#include "HTTPServer.h"
#include "Provision.h"
#include "MQTTClient.h"
#include "DefaultEventLoop.h"
#include "Task.h"

/* This should be singleton */
/* This should be derived from the WiFi and others */
// class Device :public WiFi, public EventLoop

class Device : public NVStorage, public DefaultEventLoop, public MQTTClient
{
private:
    std::string mName;
    std::vector<Feature *> mFeatures;
    std::string mMQTTURI;
    std::string mMQTTClient;
    std::string mDeviceSubTopic;
    std::string mDevicePubTopic;
    static void DeviceEventHandler(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    static void MQTTEventHandlerDevice(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    Task * resetButtonTask;
public:
    enum
    {
        DEVICE_EVENT_NONE,
        DEVICE_EVENT_DATA,
        DEVICE_EVENT_INVALID,
        DEVICE_EVENT_DATA_RECEIVE,
        DEVICE_EVENT_DATA_SEND,
        DEVICE_EVENT_CONNECTED,
        DEVICE_EVENT_DISCONNECTED,
        DEVICE_EVENT_MAX
    };
    Device(std::string name);
    ~Device();
    Device * AddFeature(Feature * feature);
    Device * RemoveFeature(Feature * feature);
    int GetFeatureCount()
    {
        return mFeatures.size();
    }
    Feature * GetFeature(int index);
    /**/
    // void PostEvent(int32_t eventId);
    void PrintFeatures();

    void EraseData()
    {
        NVStorage::Erase("SSID");
        NVStorage::Erase("PSK");
        NVStorage::Erase("APIKey");
        NVStorage::Erase("InitEndp");
    }
    static void ResetButtonTask(void * arg);

    friend class Provision;
};

#endif
