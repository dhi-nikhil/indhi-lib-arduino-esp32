#ifndef PING_H_
#define PING_H_

#include "PingEventGroup.h"
#include "ping/ping_sock.h"

/**
 * @brief Ping class
 *
 */
class Ping : public PingEventGroup
{
private:
    /* Ping Callbacks */
    /**
     * @brief Ping success callback
     *
     * @param pingHandle handle of the ping
     * @param handlerArgument argument of the handler
     */
    static void mPingSucessCb(esp_ping_handle_t pingHandle, void * handlerArgument);
    /**
     * @brief Ping Timeout callback
     *
     * @param pingHandle handle of the ping
     * @param handlerArgument argument of the handler
     */
    static void mPingTimeoutCb(esp_ping_handle_t pingHandle, void * handlerArgument);
    /**
     * @brief Ping End callback
     *
     * @param pingHandle handle of the ping
     * @param handlerArgument argument of the handler
     */
    static void mPingEndCb(esp_ping_handle_t pingHandle, void * handlerArgument);

    esp_ping_callbacks_t mPingCallback; /* Ping callback */
    esp_ping_handle_t mPingHandle; /* Ping handle */
    esp_ping_config_t mPingConfig; /* Ping configuration */

public:
    /**
     * @brief Ping Status
     *
     */
    enum PingStatus
    {
        PING_IDLE, /* Ping Idle state */
        PING_PROCESSING, /* Pinging */
        PING_PROCESSED, /* Ping complete */
        PING_FAILED, /* Ping failed */
        PING_SUCCESS, /* Ping success */
        PING_TIMEOUT, /* Ping Timeout */
        PING_INVALID, /* Ping invalid state */
        PING_MAX,
    };
    /**
     * @brief Construct a new Ping object
     *
     */
    Ping();
    /**
     * @brief Destroy the Ping object
     *
     */
    ~Ping();
    /**
     * @brief Get the Ping Status object
     *
     * @return PingStatus status of the ping
     */
    PingStatus GetPingStatus();
    /**
     * @brief Ping IPv4 address
     *
     * @param ipAddress ipv4 address
     * @param waitMs wait for the ping
     * @return PingStatus status of the ping
     */
    PingStatus PingIPv4Address(std::string ipAddress, int waitMs);
    /**
     * @brief Ping IPv6 address
     *
     * @param ipAddress ipv6 address
     * @param waitMs wait for the ping
     * @return PingStatus status of the ping
     */
    PingStatus PingIPv6Address(std::string ipAddress, int waitMs);
};

#endif
