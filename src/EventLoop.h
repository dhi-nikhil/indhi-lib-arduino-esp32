#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string"
#include "esp_event_base.h"
#include "esp_event.h"

// namespace Idf
// {
//     namespace EventLoop
//     {
/**
 * @brief Class for event loop
 *
 */
class EventLoop
{
private:
    /**
     * @brief Task handle
     *
     */
    TaskHandle_t mTaskHandle;
    /**
     * @brief Event loop arguments
     *
     */
    esp_event_loop_args_t mEventLoopArgs;
    /**
     * @brief Event loop handle
     *
     */
    esp_event_loop_handle_t mEventLoopHandle;
    /**
     * @brief Event loop handler
     *
     */
    esp_event_handler_t mEventHandler;
    /**
     * @brief Event loop instance
     *
     */
    esp_event_handler_instance_t mEventHandlerInstance;

public:
    /**
     * @brief Test events
     *
     */
    enum
    {
        USEREVENTONE,
        USEREVENTTWO,
        USEREVENTTHREE
    };

    /**
     * @brief Name of the event loop
     *
     */
    std::string mEventLoopName;
    /**
     * @brief Event loop base
     *
     */
    std::string mEventBase;
    /**
     * @brief Event loop context
     *
     */
    void * mContext;
    /**
     * @brief Construct a new Event Loop object
     *
     */
    EventLoop();
    /**
     * @brief Construct a new Event Loop object
     *
     * @param eventBase event base with which we are creating the event loop
     * @param taskName name of the task for event loop
     * @param queueSize size of the event loop
     * @param taskPriority priority of the event loop task
     * @param coreId core to which this event loop task pinned with
     * @param eventHandler event handler function
     * @param context context to pass to the handler
     */
    EventLoop(std::string eventBase, std::string taskName, int32_t queueSize, int taskPriority, BaseType_t coreId, esp_event_handler_t eventHandler, void * context = nullptr);
    /**
     * @brief Create the event loop
     *
     * @return esp_err_t succes or fail
     */
    esp_err_t Create();
    /**
     * @brief Delete the event loop
     *
     * @return esp_err_t success or fail
     */
    esp_err_t Delete();
    /**
     * @brief Post the events to the event loop
     *
     * @param eventId id with to post event with
     * @return esp_err_t return success or fail
     */
    esp_err_t Post(int32_t eventId);
    /**
     * @brief Post event with data
     *
     * @param eventId id with to post event with
     * @param eventData data to post event with
     * @return esp_err_t return success or fail
     */
    esp_err_t Post(int32_t eventId, void * eventData);
    /**
     * @brief Post event with data
     *
     * @param eventId id with to post event with
     * @param eventData data to post event with
     * @param eventDataSize size of the data
     * @return esp_err_t return success or fail
     */
    esp_err_t Post(int32_t eventId, void * eventData, size_t eventDataSize);
    /**
     * @brief Register the handler with event id
     *
     * @param eventId event id to register
     * @return esp_err_t return success or fail
     */
    esp_err_t RegisterHandler(int32_t eventId = ESP_EVENT_ANY_ID);
    /**
     * @brief Register the handle with event id and base
     *
     * @param eventBase event base to register
     * @param eventId event id to register
     * @return esp_err_t return success or fail
     */
    esp_err_t RegisterHandler(esp_event_base_t eventBase, int32_t eventId);
    /**
     * @brief Unregister the event id
     *
     * @param eventId event id to unregister
     * @return esp_err_t return success or fail
     */
    esp_err_t UnregisterHandler(int32_t eventId);
    /**
     * @brief Unregister the event id and base
     *
     * @param eventBase event base to unregister
     * @param eventId event id to unregister
     * @return esp_err_t return success or fail
     */
    esp_err_t UnregisterHandler(esp_event_base_t eventBase, int32_t eventId);
    /**
     * @brief Destroy the Event Loop object
     *
     */
    ~EventLoop();
};

//* check if it can be implemeted using event loop above only *//
// class DefaultEventLoop : public EventLoop
// {
// private:
//     /* data */
// public:
//     enum Events
//     {
//         DEFAULTEVENTONE,
//         DEFAULTEVENTTWO,
//         DEFAULTEVENTTHREE
//     };

//     esp_event_base_t mEventBase;

//     DefaultEventLoop();
//     esp_err_t Create() override;
//     esp_err_t Delete() override;
//     esp_err_t Post(int32_t eventId) override;
//     esp_err_t RegisterHandler(int32_t eventId);
//     esp_err_t RegisterHandler(esp_event_base_t eventBase, int32_t eventId);
//     esp_err_t UnregisterHandler(int32_t eventId); // Override in derived
//     esp_err_t UnregisterHandler(esp_event_base_t eventBase, int32_t eventId);
//     static void TaskEventDefaultHandler(void *handlerArgs, esp_event_base_t base, int32_t id, void *eventData);
//     ~DefaultEventLoop();
// };
//     }
// }

#endif
