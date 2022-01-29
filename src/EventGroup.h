#ifndef EVENT_GROUP_H_
#define EVENT_GROUP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "string.h"
#include "string"

/* base class each class will derive with the declaration from this */
class EventGroup
{
private:
    EventGroupHandle_t mEventGroupHandle;
public:
    EventGroup();
    EventBits_t EventGroupWaitBits(EventBits_t eventBits, BaseType_t clearOnExit = false, BaseType_t waitForAllBits = true, TickType_t tick = 0);
    EventBits_t EventGroupClearBits(EventBits_t eventBits);
    BaseType_t EventGroupClearBitsFromISR(EventBits_t eventBits);
    bool IsEventGroupBitSet(EventBits_t eventBit);
    bool IsEventGroupBitClear(EventBits_t eventBit);
    EventBits_t EventGroupSetBits(EventBits_t eventBits);
    BaseType_t EventGroupSetBits(EventBits_t eventBits, BaseType_t highPriorityTaskWoken);
    BaseType_t EventGroupSetBitsFromISR(EventBits_t eventBits, BaseType_t highPriorityTaskWoken);
    EventBits_t EventGroupSyncBits(EventBits_t setEventBits, EventBits_t waitEventBits, TickType_t tick);
    EventBits_t EventGroupGetBits();
    EventBits_t EventGroupGetBitsFromISR();
    void EventGroupDelete();
    ~EventGroup();
};

class test_event_group : public EventGroup
{
public:
    test_event_group() : EventGroup()
    {

    }
    ~test_event_group()
    {

    }
    static const EventBits_t EVENT_ONE = BIT0;
    static const EventBits_t EVENT_TWO = BIT1;
    static const EventBits_t EVENT_THREE = BIT2;
};

#endif
