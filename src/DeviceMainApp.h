#ifndef DEVICE_MAIN_APP_H_
#define DEVICE_MAIN_APP_H_

#include "FeatureMainApp.h"
#include "string"
#include "vector"
#include "WiFi.h"
#include "EventLoop.h"
#include "HTTPServer.h"
#include "ProvisionMainApp.h"
#include "MQTTClient.h"
#include "DefaultEventLoop.h"
#include "Task.h"

/* This should be singleton */
/* This should be derived from the WiFi and others */
// class Device :public WiFi, public EventLoop

class DeviceMainApp : public NVStorage, public DefaultEventLoop, public MQTTClient
{
private:
    std::string mName;
    std::vector<FeatureMainApp *> mFeatures;
    std::string mMQTTURI;
    std::string mMQTTClient;
    std::string mDeviceSubTopic;
    std::string mDevicePubTopic;
    static void DeviceEventHandler(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    static void MQTTEventHandlerDevice(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    Task * resetButtonTask;

    /* Main app details */
    bool mIsMainAppDevice;
    std::string mManufacturerId;
    std::string mModel;
    std::string mDeviceId;
    std::string mDeviceType;
    std::string mSerialNumber;
    std::string mAPSSID;
    std::string mAPPSK;
    std::string mCurrentFirmwareVersion;
    std::string mBaseMACID;
    std::string mSTAMACID;
    std::string mAPMACID;
    std::string mBLEMACID;

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
    /* Main app */
    DeviceMainApp(std::string manufacturerId, std::string model, std::string deviceId, std::string deviceType, std::string serialNumber, std::string apSSID, std::string apPSK, std::string staMACAddress, std::string apMACAddress, std::string bleMACAddress);
    ~DeviceMainApp();
    DeviceMainApp * AddFeature(FeatureMainApp * feature);
    DeviceMainApp * RemoveFeature(FeatureMainApp * feature);
    int GetFeatureCount()
    {
        return mFeatures.size();
    }
    FeatureMainApp * GetFeature(int index);
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

    friend class ProvisionMainApp;
};

#endif
