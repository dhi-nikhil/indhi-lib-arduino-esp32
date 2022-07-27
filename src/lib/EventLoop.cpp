#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "EventLoop.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_event_base.h"

static const char * TAG = "EventLoop";

// namespace Idf
// {
//     namespace EventLoop
//     {
EventLoop::EventLoop()
{
}

EventLoop::EventLoop(std::string eventBase, std::string taskName, int32_t queueSize, int taskPriority, BaseType_t coreId, esp_event_handler_t eventHandler, void * context)
{
    mEventLoopName = taskName;
    mEventHandler = eventHandler;
    mContext = context;
    mEventLoopArgs.queue_size = queueSize;
    mEventLoopArgs.task_core_id = 0;
    mEventLoopArgs.task_name = taskName.c_str();
    mEventLoopArgs.task_priority = taskPriority;
    mEventLoopArgs.task_stack_size = 8192;
    mEventBase = eventBase;
    ESP_ERROR_CHECK_WITHOUT_ABORT(Create());
    ESP_ERROR_CHECK_WITHOUT_ABORT(RegisterHandler());
}

EventLoop::~EventLoop()
{
    /* other defined base should be unregistered in the users code */
    ESP_LOGI(TAG, "Deleting Event Loop %s", mEventLoopName.c_str());
    ESP_ERROR_CHECK_WITHOUT_ABORT(UnregisterHandler(ESP_EVENT_ANY_ID));
    ESP_ERROR_CHECK_WITHOUT_ABORT(Delete());
}

esp_err_t EventLoop::Create()
{
    ESP_LOGI(TAG, "Created Event Loop with name %s", mEventLoopName.c_str());
    return esp_event_loop_create(&mEventLoopArgs, &mEventLoopHandle);
}

esp_err_t EventLoop::Delete()
{
    ESP_LOGI(TAG, "Deleted Event Loop with name %s", mEventLoopName.c_str());
    return esp_event_loop_delete(mEventLoopHandle);
}

esp_err_t EventLoop::RegisterHandler(int32_t eventId)
{
    return esp_event_handler_instance_register_with(mEventLoopHandle, mEventBase.c_str(), eventId, mEventHandler, mContext, &mEventHandlerInstance);
}

esp_err_t EventLoop::RegisterHandler(esp_event_base_t eventBase, int32_t eventId)
{
    return esp_event_handler_instance_register_with(mEventLoopHandle, eventBase, eventId, mEventHandler, mContext, &mEventHandlerInstance);
}

esp_err_t EventLoop::UnregisterHandler(int32_t eventId)
{
    return esp_event_handler_instance_unregister_with(mEventLoopHandle, mEventBase.c_str(), eventId, mEventHandlerInstance); //  some error here with instance SOMETHING WITH POINTER
}

esp_err_t EventLoop::UnregisterHandler(esp_event_base_t eventBase, int32_t eventId)
{
    ESP_LOGI(TAG, "Posting Event %d with Base %s", eventId, eventBase);
    return esp_event_handler_instance_unregister_with(mEventLoopHandle, eventBase, eventId, mEventHandlerInstance);
}

esp_err_t EventLoop::Post(int32_t eventId)
{
    return esp_event_post_to(mEventLoopHandle, mEventBase.c_str(), eventId, mContext, sizeof(mContext), portMAX_DELAY);
}

esp_err_t EventLoop::Post(int32_t eventId, void * eventData)
{
    return esp_event_post_to(mEventLoopHandle, mEventBase.c_str(), eventId, eventData, sizeof(eventData), portMAX_DELAY);
}

esp_err_t EventLoop::Post(int32_t eventId, void * eventData, size_t eventDataSize)
{
    return esp_event_post_to(mEventLoopHandle, mEventBase.c_str(), eventId, eventData, eventDataSize, portMAX_DELAY);
}

/*************************************************************/

// DefaultEventLoop::DefaultEventLoop() : EventLoop()
// {
//     ESP_LOGI(TAG, "Default Event Loop Base Constructed");
// }

// esp_err_t DefaultEventLoop::Create()
// {
//     return esp_event_loop_create_default();
// }

// esp_err_t DefaultEventLoop::Delete()
// {
//     return esp_event_loop_delete_default();
// }

// esp_err_t DefaultEventLoop::Post(int32_t eventId)
// {
//     return esp_event_post(mEventBase, eventId, nullptr, 0, pdMS_TO_TICKS(0));
// }

// esp_err_t DefaultEventLoop::RegisterHandler(int32_t eventId)
// {
//     return esp_event_handler_instance_register(mEventBase, eventId, TaskEventDefaultHandler, (char *)"Default", &mEventHandlerInstance);
// }

// esp_err_t DefaultEventLoop::RegisterHandler(esp_event_base_t eventBase, int32_t eventId)
// {
//     return esp_event_handler_instance_register(mEventBase, eventId, TaskEventDefaultHandler, (char *)"Default", &mEventHandlerInstance);
// }

// esp_err_t DefaultEventLoop::RegisterHandler(esp_event_base_t eventBase, int32_t eventId, esp_event_handler_t eventHandler, void *event_handler_arg, esp_event_handler_instance_t *instance)
// {
//     return esp_event_handler_instance_register(mEventBase, eventId, eventHandler, event_handler_arg, instance);
// }

// esp_err_t DefaultEventLoop::UnregisterHandler(int32_t eventId)
// {
//     return esp_event_handler_instance_unregister(mEventBase, eventId, mEventHandlerInstance);
// }

// esp_err_t DefaultEventLoop::UnregisterHandler(esp_event_base_t eventBase, int32_t eventId)
// {
//     return esp_event_handler_instance_unregister(eventBase, eventId, mEventHandlerInstance);
// }

// esp_err_t DefaultEventLoop::UnregisterHandler(esp_event_base_t eventBase, int32_t eventId, esp_event_handler_instance_t instance)
// {
//     return esp_event_handler_instance_unregister(eventBase, eventId, instance);
// }

// void DefaultEventLoop::TaskEventDefaultHandler(void *handlerArgs, esp_event_base_t base, int32_t id, void *eventData)
// {
//     ESP_LOGI(TAG, "Context : %s, Event Base : %s, Event Id : %d", (char *)handlerArgs, base, id);
// }

// DefaultEventLoop::~DefaultEventLoop()
// {
//     ESP_LOGI(TAG, "User Event Loop Base Distructed");
// }
//     }
// }
