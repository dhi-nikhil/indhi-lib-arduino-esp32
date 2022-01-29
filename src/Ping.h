#ifndef PING_H_
#define PING_H_

#include "PingEventGroup.h"
#include "ping/ping_sock.h"

class Ping : public PingEventGroup
{
private:
    /* Ping Callbacks */
    static void mPingSucessCb(esp_ping_handle_t pingHandle, void * handlerArgument);
    static void mPingTimeoutCb(esp_ping_handle_t pingHandle, void * handlerArgument);
    static void mPingEndCb(esp_ping_handle_t pingHandle, void * handlerArgument);

    esp_ping_callbacks_t mPingCallback;
    esp_ping_handle_t mPingHandle;
    esp_ping_config_t mPingConfig;

public:
    enum PingStatus
    {
        PING_IDLE,
        PING_PROCESSING,
        PING_PROCESSED,
        PING_FAILED,
        PING_SUCCESS,
        PING_TIMEOUT,
        PING_INVALID,
        PING_MAX,
    };
    Ping();
    ~Ping();
    PingStatus GetPingStatus();
    PingStatus PingIPv4Address(std::string ipAddress, int waitMs);
    PingStatus PingIPv6Address(std::string ipAddress, int waitMs);
};

#endif
