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
class EventLoop
{
private:
    TaskHandle_t mTaskHandle;
    esp_event_loop_args_t mEventLoopArgs;
    esp_event_loop_handle_t mEventLoopHandle;
    esp_event_handler_t mEventHandler;
    esp_event_handler_instance_t mEventHandlerInstance;

public:
    enum
    {
        USEREVENTONE,
        USEREVENTTWO,
        USEREVENTTHREE
    };

    std::string mEventLoopName;
    std::string mEventBase;
    void * mContext;
    EventLoop();
    EventLoop(std::string eventBase, std::string taskName, int32_t queueSize, int taskPriority, BaseType_t coreId, esp_event_handler_t eventHandler, void * context = nullptr);
    esp_err_t Create();
    esp_err_t Delete();
    esp_err_t Post(int32_t eventId);
    esp_err_t Post(int32_t eventId, void * eventData);
    esp_err_t Post(int32_t eventId, void * eventData, size_t eventDataSize);
    esp_err_t RegisterHandler(int32_t eventId = ESP_EVENT_ANY_ID);
    esp_err_t RegisterHandler(esp_event_base_t eventBase, int32_t eventId);
    esp_err_t UnregisterHandler(int32_t eventId);
    esp_err_t UnregisterHandler(esp_event_base_t eventBase, int32_t eventId);
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
