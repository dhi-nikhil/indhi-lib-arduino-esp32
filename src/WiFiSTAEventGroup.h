#ifndef WIFI_STA_EVENT_GROUP_H_
#define WIFI_STA_EVENT_GROUP_H_

#include "EventGroup.h"

class WiFiSTAEventGroup : public EventGroup
{
private:

public:
    static const EventBits_t EG_STA_IDLE = BIT0;
    static const EventBits_t EG_STA_CONNECTED = BIT1;
    static const EventBits_t EG_STA_CONNECTING = BIT2;
    static const EventBits_t EG_STA_NOT_CONNECTED = BIT3;
    static const EventBits_t EG_STA_CONNECT = BIT4;
    static const EventBits_t EG_STA_CONNECT_FAILED = BIT5;
    static const EventBits_t EG_STA_DISCONNECT = BIT6;
    static const EventBits_t EG_STA_START = BIT7;
    static const EventBits_t EG_STA_STOP = BIT8;

    WiFiSTAEventGroup() : EventGroup()
    {

    }

    ~WiFiSTAEventGroup()
    {

    }

};

#endif
