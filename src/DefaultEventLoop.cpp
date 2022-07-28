#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "DefaultEventLoop.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char * TAG = "DefaultEventLoop";

DefaultEventLoop::DefaultEventLoop()
{
    // ESP_LOGI(TAG, "Default Event Loop Created with Constructor");
    // esp_event_loop_create_default();
    mDefaultEventHandlerInstance = NULL;
}

DefaultEventLoop::~DefaultEventLoop()
{
    // ESP_LOGI(TAG, "Default Event Loop Deleted with Distructor");
    // esp_event_loop_delete_default();
}

esp_err_t DefaultEventLoop::Create()
{
    ESP_LOGI(TAG, "Default Event Loop Created method");
    return ::esp_event_loop_create_default();
}

esp_err_t DefaultEventLoop::Delete()
{
    ESP_LOGI(TAG, "Default Event Loop Deleted with static method");
    // return ::esp_event_loop_delete_default();
    return ESP_OK;
}

esp_err_t DefaultEventLoop::Post(esp_event_base_t eventBase, int32_t eventId, void * eventData, size_t eventDataSize, TickType_t tickToWait)
{
    return ::esp_event_post(eventBase, eventId, eventData, eventDataSize, tickToWait);
}

esp_err_t DefaultEventLoop::PostFromISR(esp_event_base_t eventBase, int32_t eventId, void * eventData, size_t eventDataSize, BaseType_t * taskUnblocked)
{
    return ::esp_event_isr_post(eventBase, eventId, eventData, eventDataSize, taskUnblocked);
}

esp_err_t DefaultEventLoop::DefaultEventLoopCreate()
{
    ESP_LOGI(TAG, "Default Event Loop Created with static method");
    return ::esp_event_loop_create_default();
}

esp_err_t DefaultEventLoop::DefaultEventLoopDelete()
{
    ESP_LOGI(TAG, "Default Event Loop Deleted with static method");
    // return ::esp_event_loop_delete_default();
    return ESP_OK;
}

