#ifndef WIFI_SCAN_EVENT_GROUP_H_
#define WIFI_SCAN_EVENT_GROUP_H_

#include "EventGroup.h"

class WiFiScanEventGroup : public EventGroup
{
private:

public:
    static const EventBits_t EG_SCAN_IDLE = BIT0;
    static const EventBits_t EG_SCAN_STARTED = BIT1;
    static const EventBits_t EG_SCAN_COMPLETED = BIT2;
    static const EventBits_t EG_SCAN_INPROCESS = BIT3;
    static const EventBits_t EG_SCAN_ERROR = BIT4;

    WiFiScanEventGroup() : EventGroup()
    {

    }

    ~WiFiScanEventGroup()
    {

    }

};

#endif
