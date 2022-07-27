#ifndef WIFI_AP_EVENT_GROUP_H_
#define WIFI_AP_EVENT_GROUP_H_

#include "EventGroup.h"

/**
 * @brief WiFi event group
 *
 */
class WiFiAPEventGroup : public EventGroup
{
private:

public:
    static const EventBits_t EG_AP_IDLE = BIT0; /* AP idle state */
    static const EventBits_t EG_AP_START = BIT1; /* AP start state */
    static const EventBits_t EG_AP_STOP = BIT2; /* AP stop state */
    static const EventBits_t EG_AP_STA_CONNECTED = BIT3; /* ap connected state */
    static const EventBits_t EG_AP_STA_DISCONNECTED = BIT4; /* ap disconnected state */

    /**
     * @brief Construct a new Wi Fi A P Event Group object
     *
     */
    WiFiAPEventGroup() : EventGroup()
    {

    }

    /**
     * @brief Destroy the Wi Fi A P Event Group object
     *
     */
    ~WiFiAPEventGroup()
    {

    }

};

#endif
