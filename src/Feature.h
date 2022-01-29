#ifndef FEATURE_H_
#define FEATURE_H_

#include "DefaultEventLoop.h"

typedef enum
{
    FEATURE_TYPE_PUB = 1,
    FEATURE_TYPE_PUBSUB = 2
}FeatureType_t;

typedef enum
{
    WIDGET_CODE_NONE,
    WIDGET_CODE_TEXT,
    WIDGET_CODE_ONOFF,
    WIDGET_CODE_LEVEL_BAR,
    WIDGET_CODE_COLOR_PALLET,
    WIDGET_CODE_INVALID,
    WODGET_CODE_MAX
}WidgetCode_t;

typedef struct
{

} WidgetData_t;


class FeatureHTTPRequest
{
private:

public:
    FeatureHTTPRequest(/* args */);
    ~FeatureHTTPRequest();
};

class Feature;

typedef esp_err_t (*featureCallback_t)( Feature * feature, double data );

class Feature :public DefaultEventLoop
{
private:
    // friend class Device;
    // EventLoop * mEventLoop;
    FeatureType_t mFeatureType;
    WidgetCode_t mWidgetCode;
    WidgetData_t mWidgetData;
    int mVersionCode;
    featureCallback_t mFeatureCallback;
    static void FeatureEventHandler(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData);
    std::string mFeatureName;
    std::string mSubTopic;
    std::string mPubTopic;
    /* store data here */
    std::string mDataRX;
    std::string mDataTX;

protected:
    int mOnValue;
    int mOffValue;
    int mMinLevel;
    int mMaxLevel;

    friend class Provision;
    friend class Device;

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
    Feature(std::string id, std::string featureName, FeatureType_t featureType, WidgetCode_t widgetCode, int versionCode, featureCallback_t featureCallback);
    // ~Feature();
};

class TextFeature :public Feature
{
private:
    /* data */
public:
    TextFeature(std::string id, std::string featureName, int versionCode) :
        Feature(id, featureName, FeatureType_t::FEATURE_TYPE_PUB, WidgetCode_t::WIDGET_CODE_TEXT, versionCode, nullptr)
    {
    }

    // ~TextFeature() {}
};

class OnOffLightFeature :public Feature
{
private:
public:
    OnOffLightFeature(std::string id, std::string featureName, int versionCode, int onValue, int offValue, featureCallback_t featureCallback) :
        Feature(id, featureName, FeatureType_t::FEATURE_TYPE_PUBSUB, WidgetCode_t::WIDGET_CODE_ONOFF, versionCode, featureCallback)
    {
        mOnValue = onValue;
        mOffValue = offValue;
    }

    // ~OnOffLightFeature() {}
};

class LevelBarFeature :public Feature
{
private:
public:
    LevelBarFeature(std::string id, std::string featureName, int versionCode, int minLevel, int maxLevel, int steps, featureCallback_t featureCallback) :
        Feature(id, featureName, FeatureType_t::FEATURE_TYPE_PUBSUB, WidgetCode_t::WIDGET_CODE_LEVEL_BAR, versionCode, featureCallback)
    {
        mMinLevel = minLevel;
        mMaxLevel = maxLevel;
    }

    // ~LevelBarFeature() {}
};

class ColorPalletFeature :public Feature
{
private:
    /* data */
public:
    ColorPalletFeature(std::string id, std::string featureName, int versionCode) :
        Feature(id, featureName, FeatureType_t::FEATURE_TYPE_PUBSUB, WidgetCode_t::WIDGET_CODE_COLOR_PALLET, versionCode, nullptr)
    {
    }

    // ~ColorPalletFeature() {}
};

#endif
