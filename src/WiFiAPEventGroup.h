#ifndef WIFI_AP_EVENT_GROUP_H_
#define WIFI_AP_EVENT_GROUP_H_

#include "EventGroup.h"

class WiFiAPEventGroup : public EventGroup
{
private:

public:
    static const EventBits_t EG_AP_IDLE = BIT0;
    static const EventBits_t EG_AP_START = BIT1;
    static const EventBits_t EG_AP_STOP = BIT2;
    static const EventBits_t EG_AP_STA_CONNECTED = BIT3;
    static const EventBits_t EG_AP_STA_DISCONNECTED = BIT4;

    WiFiAPEventGroup() : EventGroup()
    {

    }

    ~WiFiAPEventGroup()
    {

    }

};

#endif
