#ifndef WIFI_EVENT_GROUP_H_
#define WIFI_EVENT_GROUP_H_

#include "EventGroup.h"

/**
 * @brief WiFI event group ( not used )
 *
 */
class WiFiEventGroup : public EventGroup
{
private:

public:

    // static const EventBits_t EG_WIFI_SCAN_IDLE = BIT0;
    // static const EventBits_t EG_WIFI_SCAN_COMPLETED = BIT1;
    // static const EventBits_t EG_WIFI_SCAN_INPROCESS = BIT2;
    // static const EventBits_t EG_WIFI_SCAN_FAILED = BIT3;

    /**
     * @brief Construct a new Wi Fi Event Group object
     *
     */
    WiFiEventGroup() : EventGroup()
    {

    }

    /**
     * @brief Destroy the Wi Fi Event Group object
     *
     */
    ~WiFiEventGroup()
    {

    }

};

#endif
