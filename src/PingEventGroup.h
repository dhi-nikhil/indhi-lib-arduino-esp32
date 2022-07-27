#ifndef PING_EVENT_GROUP_H_
#define PING_EVENT_GROUP_H_

#include "EventGroup.h"

/**
 * @brief Event group for ping
 *
 */
class PingEventGroup :public EventGroup
{
private:

public:
    static const EventBits_t EG_PING_IDLE = BIT0; /* Ping idle state */
    static const EventBits_t EG_PING_PROCESSING = BIT1; /* Ping processing */
    static const EventBits_t EG_PING_PROCESSED = BIT2; /* Ping processed */
    static const EventBits_t EG_PING_FAILED = BIT3; /* Ping failed */
    static const EventBits_t EG_PING_SUCCESS = BIT4; /* Ping success */
    static const EventBits_t EG_PING_TIMEOUT = BIT5; /* Ping timeout */
    static const EventBits_t EG_PING_INVALID = BIT6; /* Ping invalid */

    /**
     * @brief Construct a new Ping Event Group object
     *
     */
    PingEventGroup() : EventGroup()
    {

    }

    /**
     * @brief Destroy the Ping Event Group object
     *
     */
    ~PingEventGroup()
    {

    }
};

#endif
