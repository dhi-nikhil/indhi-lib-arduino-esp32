#ifndef PROVISION_EVENT_GROUP_H_
#define PROVISION_EVENT_GROUP_H_

#include "EventGroup.h"

/**
 * @brief Event group class for managing events during onboarding process. It is derived from Event group base class
 *
 */
class ProvisionEventGroup : public EventGroup
{
private:

public:
    /**
     * @brief When the onboarding process is initialized and when the class is initialized
     *
     */
    static const EventBits_t EG_PROVISION_IDLE = BIT0;
    /**
     * @brief To indicate if onboarding process is started or not
     *
     */
    static const EventBits_t EG_PROVISION_STARTED = BIT1;
    /**
     * @brief Not used
     *
     */
    static const EventBits_t EG_PROVISION_PROVISIONING = BIT2;
    /**
     * @brief To indicate if the onboarding process is stopped or not
     *
     */
    static const EventBits_t EG_PROVISION_STOPPED = BIT3;
    /**
     * @brief To indicate if onboarding is completed or not
     *
     */
    static const EventBits_t EG_PROVISION_COMPLETED = BIT4;
    /**
     * @brief To indicate if the configuration is received from mobile App during onboarding using AP mode
     *
     */
    static const EventBits_t EG_PROVISION_AP_HTTP_SERVER_COMPLETED = BIT5;
    /**
     * @brief To indicate if the provisionnig MQTT client is connected to MQTT or not
     *
     */
    static const EventBits_t EG_PROVISION_MQTT_EVENT_CONNECTED = BIT6;
    /**
     * @brief To indicate if the provisionnig MQTT client is disconnected from MQTT or not
     *
     */
    static const EventBits_t EG_PROVISION_MQTT_EVENT_DISCONNECTED = BIT7;
    /**
     * @brief To indicate if the activation process is success or not
     *
     */
    static const EventBits_t EG_PROVISION_ACTIVATION_SUCCESS = BIT8;
    /**
     * @brief To indicate if the activation process is failed or not
     *
     */
    static const EventBits_t EG_PROVISION_ACTIVATION_FAILED = BIT9;
    /**
     * @brief To indicate if the activation pending is sent or not
     *
     */
    static const EventBits_t EG_PROVISION_ACTIVATION_PENDING = BIT10;
    /**
     * @brief To indicate if the configuration is received from mobile App during onboarding using BLE mode
     *
     */
    static const EventBits_t EG_PROVISION_BLE_COMPLETED = BIT11;
    /**
     * @brief To indicate if the onboarding process is aborted from mobile app
     *
     */
    static const EventBits_t EG_PROVISION_ABORTED = BIT12;
    // static const EventBits_t EG_PROVISION_WIFI_SCANNING = BIT13;

    /**
     * @brief Construct a new Provision Event Group object
     *
     */
    ProvisionEventGroup() : EventGroup()
    {

    }

    /**
     * @brief Destroy the Provision Event Group object
     *
     */
    ~ProvisionEventGroup()
    {

    }

};

#endif
