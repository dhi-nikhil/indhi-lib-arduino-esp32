#ifndef FEATURE_MAIN_APP_H_
#define FEATURE_MAIN_APP_H_

#include "DefaultEventLoop.h"

class Feature;

typedef esp_err_t (*featureCallback_t)( Feature * feature, double data );

class FeatureMainApp :public DefaultEventLoop
{
private:
    // friend class Device;
    // EventLoop * mEventLoop;
    // FeatureType_t mFeatureType;
    // WidgetCode_t mWidgetCode;
    // WidgetData_t mWidgetData;
    // int mVersionCode;
    featureCallback_t mFeatureCallback;
    static void FeatureEventHandler(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    // std::string mFeatureName;
    std::string mSubTopic;
    std::string mPubTopic;
    /* store data here */
    // std::string mDataRX;
    // std::string mDataTX;

protected:

    friend class ProvisionMainApp;
    friend class DeviceMainApp;

public:

    enum
    {
        FEATURE_EVENT_NONE,
        FEATURE_EVENT_DATA,
        FEATURE_EVENT_INVALID,
        FEATURE_EVENT_DATA_RECEIVE,
        FEATURE_EVENT_DATA_SEND,
        FEATURE_EVENT_MAX
    };
    std::string mIdentifier;
    FeatureMainApp(std::string id, featureCallback_t featureCallback = nullptr);
    // ~FeatureMainApp();
};

#endif
