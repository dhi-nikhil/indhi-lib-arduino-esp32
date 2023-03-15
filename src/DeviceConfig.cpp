#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "DeviceConfig.h"
#include "DefaultEventLoop.h"
#include "string"
#include "esp_log.h"

static const char * TAG = "DeviceConfig";

DeviceConfig::DeviceConfig(std::string var, double value)
{
    mVar = var;
    mValue = value;
}
