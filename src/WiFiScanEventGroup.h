#ifndef WIFI_SCAN_EVENT_GROUP_H_
#define WIFI_SCAN_EVENT_GROUP_H_

#include "EventGroup.h"

class WiFiScanEventGroup : public EventGroup
{
private:

public:
    static const EventBits_t EG_SCAN_IDLE = BIT0; /* Scan state idle */
    static const EventBits_t EG_SCAN_STARTED = BIT1; /* Scan state started */
    static const EventBits_t EG_SCAN_COMPLETED = BIT2; /* scan state completed */
    static const EventBits_t EG_SCAN_INPROCESS = BIT3; /* scan state in progress */
    static const EventBits_t EG_SCAN_ERROR = BIT4; /* scan state error */

    /**
     * @brief Construct a new Wi Fi Scan Event Group object
     *
     */
    WiFiScanEventGroup() : EventGroup()
    {

    }

    /**
     * @brief Destroy the Wi Fi Scan Event Group object
     *
     */
    ~WiFiScanEventGroup()
    {

    }

};

#endif
