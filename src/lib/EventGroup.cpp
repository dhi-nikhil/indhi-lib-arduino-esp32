#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "EventGroup.h"
#include "esp_log.h"

static const char * TAG = "EventGroup";

EventGroup::EventGroup()
{
    ESP_LOGI(__func__, "initialized event group");
    mEventGroupHandle = xEventGroupCreate();
    if (mEventGroupHandle == NULL)
    {
        ESP_LOGE(__func__, "Event Group Initialization Failed");
    }
    // ESP_LOGD(TAG, "Pointer %p", mEventGroupHandle);
}

EventBits_t EventGroup::EventGroupWaitBits(EventBits_t eventBits, BaseType_t clearOnExit, BaseType_t waitForAllBits, TickType_t tick)
{
    ESP_LOGI(TAG, "Waiting for Bits : %08x", eventBits);
    return xEventGroupWaitBits(mEventGroupHandle, eventBits, clearOnExit, waitForAllBits, tick);
}

EventBits_t EventGroup::EventGroupClearBits(EventBits_t eventBits)
{
    ESP_LOGW(TAG, "Bits Clear %08x", eventBits);
    return xEventGroupClearBits(mEventGroupHandle, eventBits);
}

BaseType_t EventGroup::EventGroupClearBitsFromISR(EventBits_t eventBits)
{
    return xEventGroupClearBitsFromISR(mEventGroupHandle, eventBits);
}

bool EventGroup::IsEventGroupBitSet(EventBits_t eventBit)
{
    if (( EventGroupGetBits() & eventBit ) == eventBit)
    {
        return true;
    }
    return false;
}

bool EventGroup::IsEventGroupBitClear(EventBits_t eventBit)
{
    if (( EventGroupGetBits() & eventBit ) == eventBit)
    {
        return false;
    }
    return true;
}

EventBits_t EventGroup::EventGroupSetBits(EventBits_t eventBits)
{
    ESP_LOGW(TAG, "Bits Set %08x", eventBits);
    // if (mEventGroupHandle == NULL)
    // {
    //     ESP_LOGE(TAG, "Event Group is null");
    //     return 0;
    // }
    // ESP_LOGD(TAG, "Pointer %p", mEventGroupHandle);
    return xEventGroupSetBits(mEventGroupHandle, eventBits);
}

BaseType_t EventGroup::EventGroupSetBitsFromISR(EventBits_t eventBits, BaseType_t highPriorityTaskWoken)
{
    // ESP_LOGI(TAG, "Bits Set %d", eventBits);
    return xEventGroupSetBitsFromISR(mEventGroupHandle, eventBits, &highPriorityTaskWoken);
}

EventBits_t EventGroup::EventGroupSyncBits(EventBits_t setEventBits, EventBits_t waitEventBits, TickType_t tick)
{
    return xEventGroupSync(mEventGroupHandle, setEventBits, waitEventBits, tick);
}

EventBits_t EventGroup::EventGroupGetBits()
{
    return xEventGroupGetBits(mEventGroupHandle);
}

EventBits_t EventGroup::EventGroupGetBitsFromISR()
{
    return xEventGroupGetBitsFromISR(mEventGroupHandle);
}

void EventGroup::EventGroupDelete()
{
    vEventGroupDelete(mEventGroupHandle);
}

EventGroup::~EventGroup()
{
    ESP_LOGI(__func__, "deleted event group");
    EventGroupDelete();
}
