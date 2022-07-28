#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "Ping.h"
#include "esp_log.h"

static const char * TAG = "Ping";


void Ping::mPingSucessCb(esp_ping_handle_t pingHandle, void * handlerArgument)
{
    Ping * ping = (Ping *) handlerArgument;
    ping->PingEventGroup::EventGroupSetBits(PingEventGroup::EG_PING_SUCCESS);
    ping->PingEventGroup::EventGroupClearBits(PingEventGroup::EG_PING_TIMEOUT);
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_ping_stop(pingHandle)); //  testing 
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_ping_delete_session(pingHandle)); // testing

}

void Ping::mPingTimeoutCb(esp_ping_handle_t pingHandle, void * handlerArgument)
{
    Ping * ping = (Ping *) handlerArgument;
    ping->PingEventGroup::EventGroupSetBits(PingEventGroup::EG_PING_TIMEOUT);
}

void Ping::mPingEndCb(esp_ping_handle_t pingHandle, void * handlerArgument)
{
    Ping * ping = (Ping *) handlerArgument;
    ping->PingEventGroup::EventGroupClearBits(PingEventGroup::EG_PING_PROCESSING);
    ping->PingEventGroup::EventGroupSetBits(PingEventGroup::EG_PING_PROCESSED);
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_ping_stop(pingHandle));
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_ping_delete_session(pingHandle));
}

Ping::Ping() :PingEventGroup()
{
    mPingConfig.count = 5;
    mPingConfig.interval_ms = 100;
    mPingConfig.timeout_ms = 1000;
    mPingConfig.data_size = 64;
    mPingConfig.tos = 0;
    mPingConfig.target_addr = *( IP_ANY_TYPE );
    mPingConfig.task_stack_size = 2048;
    mPingConfig.task_prio = 2;
    mPingConfig.interface = 0;

    mPingCallback.on_ping_success = mPingSucessCb;
    mPingCallback.on_ping_timeout = mPingTimeoutCb;
    mPingCallback.on_ping_end = mPingEndCb;
    mPingCallback.cb_args = this;

    EventGroupSetBits(PingEventGroup::EG_PING_IDLE);
}

Ping::~Ping()
{

}

Ping::PingStatus Ping::GetPingStatus()
{
    if (PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_SUCCESS))
        return PingStatus::PING_SUCCESS;
    else if (PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_FAILED))
        return PingStatus::PING_FAILED;
    else if (PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_PROCESSING))
        return PingStatus::PING_PROCESSING;
    else if (PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_PROCESSED))
        return PingStatus::PING_PROCESSED;
    else if (PingEventGroup::IsEventGroupBitSet(PingEventGroup::EG_PING_TIMEOUT))
        return PingStatus::PING_TIMEOUT;
    else
        return PingStatus::PING_INVALID;
    return PingStatus::PING_INVALID;
}

Ping::PingStatus Ping::PingIPv4Address(std::string ipAddress, int waitMs)
{
    esp_err_t err;
    PingStatus pingStatus;
    mPingConfig.target_addr.type = IPADDR_TYPE_V4;
    mPingConfig.target_addr.u_addr.ip4.addr = ::ipaddr_addr(ipAddress.c_str());

    err = ::esp_ping_new_session(&mPingConfig, &mPingCallback, &mPingHandle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    PingEventGroup::EventGroupClearBits(PingEventGroup::EG_PING_IDLE | \
        PingEventGroup::EG_PING_FAILED | \
        PingEventGroup::EG_PING_SUCCESS | \
        PingEventGroup::EG_PING_PROCESSING | \
        PingEventGroup::EG_PING_PROCESSED | \
        PingEventGroup::EG_PING_TIMEOUT);

    err = ::esp_ping_start(mPingHandle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    PingEventGroup::EventGroupSetBits(PingEventGroup::EG_PING_PROCESSING);

    PingEventGroup::EventGroupWaitBits(PingEventGroup::EG_PING_PROCESSED | PingEventGroup::EG_PING_SUCCESS, false, false, pdMS_TO_TICKS(waitMs));
    pingStatus = GetPingStatus();

    // err = ::esp_ping_stop(mPingHandle);
    // ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    // err = ::esp_ping_delete_session(mPingHandle);
    // ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    return pingStatus;
}

Ping::PingStatus Ping::PingIPv6Address(std::string ipAddress, int waitMs)
{
    esp_err_t err;
    PingStatus pingStatus;
    mPingConfig.target_addr.type = IPADDR_TYPE_V6;
    mPingConfig.target_addr.u_addr.ip4.addr = ipaddr_addr(ipAddress.c_str());
    err = ::esp_ping_new_session(&mPingConfig, &mPingCallback, &mPingHandle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    PingEventGroup::EventGroupClearBits(PingEventGroup::EG_PING_IDLE | \
        PingEventGroup::EG_PING_FAILED | \
        PingEventGroup::EG_PING_SUCCESS | \
        PingEventGroup::EG_PING_PROCESSING | \
        PingEventGroup::EG_PING_PROCESSED | \
        PingEventGroup::EG_PING_TIMEOUT);

    err = ::esp_ping_start(mPingHandle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    PingEventGroup::EventGroupSetBits(PingEventGroup::EG_PING_PROCESSING);
    PingEventGroup::EventGroupWaitBits(EG_PING_SUCCESS, false, false, pdMS_TO_TICKS(waitMs));
    pingStatus = GetPingStatus();

    err = ::esp_ping_stop(mPingHandle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);

    err = ::esp_ping_delete_session(mPingHandle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    return pingStatus;
}

