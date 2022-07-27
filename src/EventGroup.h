#ifndef EVENT_GROUP_H_
#define EVENT_GROUP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "string.h"
#include "string"

/**
 * @brief base class each class will derive with the declaration from this if required the event group
 *
 */
class EventGroup
{
private:
    /**
     * @brief Event group handle
     *
     */
    EventGroupHandle_t mEventGroupHandle;
public:
    /**
     * @brief Construct a new Event Group object
     *
     */
    EventGroup();
    /**
     * @brief Wait for the event bits
     *
     * @param eventBits events bits to wait for
     * @param clearOnExit clear event bits on exit or not
     * @param waitForAllBits wait for all bits to set or not
     * @param tick wait duration
     * @return EventBits_t status
     */
    EventBits_t EventGroupWaitBits(EventBits_t eventBits, BaseType_t clearOnExit = false, BaseType_t waitForAllBits = true, TickType_t tick = 0);
    /**
     * @brief Clear the event bits
     *
     * @param eventBits event bits to clear
     * @return EventBits_t status
     */
    EventBits_t EventGroupClearBits(EventBits_t eventBits);
    /**
     * @brief Clear the event bits from ISRR
     *
     * @param eventBits event bits to clear
     * @return BaseType_t status
     */
    BaseType_t EventGroupClearBitsFromISR(EventBits_t eventBits);
    /**
     * @brief Is event group bits set or not
     *
     * @param eventBit event bits to check
     * @return boolean if success or not
     */
    bool IsEventGroupBitSet(EventBits_t eventBit);
    /**
     * @brief Is event bits clear or not
     *
     * @param eventBit event bits to check
     * @return boolean if succss or not
     */
    bool IsEventGroupBitClear(EventBits_t eventBit);
    /**
     * @brief Set the event bits
     *
     * @param eventBits event bits to set
     * @return EventBits_t status
     */
    EventBits_t EventGroupSetBits(EventBits_t eventBits);
    /**
     * @brief Set the event bits
     *
     * @param eventBits event bits to set
     * @param highPriorityTaskWoken if to wake the high priority task
     * @return BaseType_t return if success or not
     */
    BaseType_t EventGroupSetBits(EventBits_t eventBits, BaseType_t highPriorityTaskWoken);
    /**
     * @brief Set the event bits from ISR
     *
     * @param eventBits event bits to set
     * @param highPriorityTaskWoken if to wake the high priority task
     * @return BaseType_t return if success or not
     */
    BaseType_t EventGroupSetBitsFromISR(EventBits_t eventBits, BaseType_t highPriorityTaskWoken);
    /**
     * @brief Sync the event bits
     *
     * @param setEventBits sync the event bits
     * @param waitEventBits wait for the bits
     * @param tick wait duration for the bits
     * @return EventBits_t return status
     */
    EventBits_t EventGroupSyncBits(EventBits_t setEventBits, EventBits_t waitEventBits, TickType_t tick);
    /**
     * @brief Get the event bits
     *
     * @return EventBits_t status event bits
     */
    EventBits_t EventGroupGetBits();
    /**
     * @brief Get event bits from ISR
     *
     */
    EventBits_t EventGroupGetBitsFromISR();
    /**
     * @brief Delete event bits
     *
     */
    void EventGroupDelete();
    /**
     * @brief Destroy the Event Group object
     *
     */
    ~EventGroup();
};

/**
 * @brief For testing purpose
 *
 */
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
