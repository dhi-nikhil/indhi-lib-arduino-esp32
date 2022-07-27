#ifndef WIFI_STA_EVENT_GROUP_H_
#define WIFI_STA_EVENT_GROUP_H_

#include "EventGroup.h"

/**
 * @brief WiFI sta event group
 *
 */
class WiFiSTAEventGroup : public EventGroup
{
private:

public:
    static const EventBits_t EG_STA_IDLE = BIT0; /* sta state idle */
    static const EventBits_t EG_STA_CONNECTED = BIT1; /* sta state connected */
    static const EventBits_t EG_STA_CONNECTING = BIT2; /* sta state connecting */
    static const EventBits_t EG_STA_NOT_CONNECTED = BIT3; /* sta state connected */
    static const EventBits_t EG_STA_CONNECT = BIT4; /* sta state connect */
    static const EventBits_t EG_STA_CONNECT_FAILED = BIT5; /* sta state failed to connect */
    static const EventBits_t EG_STA_DISCONNECT = BIT6; /* sta state disconnected */
    static const EventBits_t EG_STA_START = BIT7; /* sta state start */
    static const EventBits_t EG_STA_STOP = BIT8; /* sta state stop */

    /**
     * @brief Construct a new Wi Fi S T A Event Group object
     *
     */
    WiFiSTAEventGroup() : EventGroup()
    {

    }

    /**
     * @brief Destroy the Wi Fi S T A Event Group object
     *
     */
    ~WiFiSTAEventGroup()
    {

    }

};

#endif
