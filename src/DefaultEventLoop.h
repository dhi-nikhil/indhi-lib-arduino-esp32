#ifndef DEFAULT_EVENT_LOOP_H_
#define DEFAULT_EVENT_LOOP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "string"

/**
 * @brief Class for default event loop
 *
 */
class DefaultEventLoop
{
private:
    /**
     * @brief event handler instance for handling event for each entity like feature and device
     *
     */
    esp_event_handler_instance_t mDefaultEventHandlerInstance;

public:
    /**
     * @brief Construct a new Default Event Loop object
     *
     */
    DefaultEventLoop();
    /**
     * @brief Create default event loop
     *
     * @return esp_err_t return if success or failed
     */
    esp_err_t Create();
    /**
     * @brief Delete default event loop
     *
     * @return esp_err_t return if success or failed
     *
     */
    esp_err_t Delete();
    /**
     * @brief Register event to default event loop
     *
     * @param eventBase event base to which default event loop will be register
     * @param eventId event id to which default event loop will be register
     * @param eventHandler event handler which will get notified of the event
     * @param eventHandlerArguments event handler argument
     * @return esp_err_t return if success or failed
     */
    esp_err_t RegisterDefaultLoopHandler(esp_event_base_t eventBase, int32_t eventId, esp_event_handler_t eventHandler, void * eventHandlerArguments)
    {
        return esp_event_handler_instance_register(eventBase, eventId, eventHandler, eventHandlerArguments, &mDefaultEventHandlerInstance);
    }
    /**
     * @brief Register even to custom event loop
     *
     * @param eventBase Event base to which custom event loop will be register
     * @param eventId Event id to which custom event loop will be register
     * @return esp_err_t return if success or failed
     */
    esp_err_t UnregisterDefaultLoopHandler(esp_event_base_t eventBase, int32_t eventId)
    {
        return esp_event_handler_instance_unregister(eventBase, eventId, &mDefaultEventHandlerInstance);
    }
    /**
     * @brief Post event to default event loop
     *
     * @param eventBase event base with which event will be posted
     * @param eventId event id with which event will be posted
     * @param eventData event data to pass with the event
     * @param eventDateSize size of data in bytes to pass to the event
     * @param tickToWait wait for event loop to get free
     * @return esp_err_t return if event posted successfully
     */
    esp_err_t Post(esp_event_base_t eventBase, int32_t eventId, void * eventData = NULL, size_t eventDateSize = 0, TickType_t tickToWait = 0);
    /**
     * @brief Post event to default event loop from ISR
     *
     * @param eventBase event base with which event will be posted
     * @param eventId event id with which event will be posted
     * @param eventData event data to pass with the event
     * @param eventDateSize size of data in bytes to pass to the event
     * @param taskUnblocked if to ubblock the task to which event is posted
     * @return esp_err_t return if success or failed
     */
    esp_err_t PostFromISR(esp_event_base_t eventBase, int32_t eventId, void * eventData, size_t eventDateSize, BaseType_t * taskUnblocked = nullptr);
    /**
     * @brief Static decalaration to create default event loop
     *
     * @return esp_err_t return if success or failed
     */
    static esp_err_t DefaultEventLoopCreate();
    /**
     * @brief Static decalration to delete default event loop
     *
     * @return esp_err_t return if success or failed
     */
    static esp_err_t DefaultEventLoopDelete();
    /**
     * @brief Destroy the Default Event Loop object
     *
     */
    ~DefaultEventLoop();
};

#endif
