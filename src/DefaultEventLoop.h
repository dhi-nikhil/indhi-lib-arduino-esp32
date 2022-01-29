#ifndef DEFAULT_EVENT_LOOP_H_
#define DEFAULT_EVENT_LOOP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "string"

class DefaultEventLoop
{
private:
    esp_event_handler_instance_t mDefaultEventHandlerInstance;

public:
    DefaultEventLoop();
    esp_err_t Create();
    esp_err_t Delete();
    esp_err_t RegisterDefaultLoopHandler(esp_event_base_t eventBase, int32_t eventId, esp_event_handler_t eventHandler, void * eventHandlerArguments)
    {
        return esp_event_handler_instance_register(eventBase, eventId, eventHandler, eventHandlerArguments, &mDefaultEventHandlerInstance);
    }
    esp_err_t UnregisterDefaultLoopHandler(esp_event_base_t eventBase, int32_t eventId)
    {
        return esp_event_handler_instance_unregister(eventBase, eventId, &mDefaultEventHandlerInstance);
    }
    esp_err_t Post(esp_event_base_t eventBase, int32_t eventId, void * eventData = NULL, size_t eventDateSize = 0, TickType_t tickToWait = 0);
    esp_err_t PostFromISR(esp_event_base_t eventBase, int32_t eventId, void * eventData, size_t eventDateSize, BaseType_t * taskUnblocked = nullptr);
    static esp_err_t DefaultEventLoopCreate();
    static esp_err_t DefaultEventLoopDelete();
    ~DefaultEventLoop();
};

#endif
