#ifndef PING_EVENT_GROUP_H_
#define PING_EVENT_GROUP_H_

#include "EventGroup.h"

class PingEventGroup :public EventGroup
{
private:

public:
    static const EventBits_t EG_PING_IDLE = BIT0;
    static const EventBits_t EG_PING_PROCESSING = BIT1;
    static const EventBits_t EG_PING_PROCESSED = BIT2;
    static const EventBits_t EG_PING_FAILED = BIT3;
    static const EventBits_t EG_PING_SUCCESS = BIT4;
    static const EventBits_t EG_PING_TIMEOUT = BIT5;
    static const EventBits_t EG_PING_INVALID = BIT6;

    PingEventGroup() : EventGroup()
    {

    }

    ~PingEventGroup()
    {

    }
};

#endif
