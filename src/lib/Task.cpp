#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Task.h"
#include "string"
#include "esp_log.h"
#include "sdkconfig.h"

static const char * TAG = "Task";

// namespace FreeRTOS
// {
//     namespace Task
//     {
Task::Task()
{
    mTaskData = nullptr;
    mTaskHandle = nullptr;
    mCoreId = tskNO_AFFINITY;
    mTaskState = eInvalid;
    mContext = nullptr;
}

Task::Task(std::string name, uint16_t stackSize, uint8_t priority, TaskFunction_t taskFunction, void * context)
{
    mTaskName = name;
    mStaskSize = stackSize;
    mPriority = priority;
    mTaskData = nullptr;
    mTaskHandle = nullptr;
    mCoreId = tskNO_AFFINITY;
    mTaskFunction = taskFunction;
    mTaskState = eInvalid;
    mContext = context;
}

Task::~Task()
{
    Delete();
    ESP_LOGI(TAG, "~Task - Task : %s is deleted", mTaskName.c_str());
}

void Task::Delay(int milliSeconds)
{
    ESP_LOGI(TAG, "Task Delay of %d", milliSeconds);
    ::vTaskDelay(milliSeconds / portTICK_PERIOD_MS);
}

void Task::Suspend()
{
    ESP_LOGI(TAG, "Task %s suspended", mTaskName.c_str());
    ::vTaskSuspend(mTaskHandle);
}

void Task::Resume()
{
    ::vTaskResume(mTaskHandle);
}

void Task::Start(void * taskData)
{
    if (mTaskHandle != nullptr)
    {
        ESP_LOGW(TAG, "start - There is some issue with already running task");
    }
    mTaskData = taskData;
    ::xTaskCreatePinnedToCore(mTaskFunction, mTaskName.c_str(), mStaskSize, this, mPriority, &mTaskHandle, mCoreId);
}

void Task::Delete()
{
    if (GetState() != eDeleted)
        ::vTaskDelete(mTaskHandle);
    else
        ESP_LOGW(TAG, "Task has already been deleted");
}

Task * Task::SetStackSize(uint16_t stackSize)
{
    mStaskSize = stackSize;
    return this;
}

Task * Task::SetPriority(uint8_t priority)
{
    mPriority = priority;
    return this;
}

Task * Task::SetName(std::string taskName)
{
    mTaskName = taskName;
    return this;
}

Task * Task::SetCore(BaseType_t coreId)
{
    mCoreId = coreId;
    return this;
}

Task * Task::SetTaskFunction(TaskFunction_t taskFunction)
{
    mTaskFunction = taskFunction;
    return this;
}

Task * Task::SetContext(void * context)
{
    mContext = context;
    return this;
}

void * Task::GetContext(void * context)
{
    return mContext;
}

TaskFunction_t Task::GetTaskFunction(TaskFunction_t taskFunction)
{
    return mTaskFunction;
}

TickType_t Task::GetTickCount()
{
    return ::xTaskGetTickCount();
}

std::string Task::GetTaskName()
{
    return std::string(pcTaskGetName(mTaskHandle));
}

void Task::ResumeFromISR()
{
    const BaseType_t xYieldRequired = ::xTaskResumeFromISR(mTaskHandle);
    portYIELD_FROM_ISR(xYieldRequired);
}

eTaskState Task::GetState()
{
    return ::eTaskGetState(mTaskHandle);
}
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
void Task::Describe()
{
    TaskStatus_t taskStatus;
    vTaskGetInfo(mTaskHandle, &taskStatus, pdTRUE, eInvalid);
    ESP_LOGI(TAG, "Current State : %d", taskStatus.eCurrentState);
    ESP_LOGI(TAG, "Task Name : %s", taskStatus.pcTaskName);
    ESP_LOGI(TAG, "Stack Base : %d", *taskStatus.pxStackBase);
    ESP_LOGI(TAG, "Run Time Counter : %d", taskStatus.ulRunTimeCounter);
    ESP_LOGI(TAG, "High Water Mark : %d", taskStatus.usStackHighWaterMark);
    ESP_LOGI(TAG, "Task Priority : %d", taskStatus.uxBasePriority);
    ESP_LOGI(TAG, "Current Priority : %d", taskStatus.uxCurrentPriority);
    ESP_LOGI(TAG, "Task Number : %d", taskStatus.xTaskNumber);
}
#endif
TaskHandle_t Task::getHandle() const
{
    return mTaskHandle;
}

uint16_t Task::GetStackSize(void)
{
    return mStaskSize;
}

UBaseType_t Task::getPriority(void)
{
    return ::uxTaskPriorityGet(mTaskHandle);
}

std::string Task::GetName(void)
{
    return mTaskName;
}

BaseType_t Task::GetCore()
{
    return mCoreId;
    //     return xTaskGetAffinity(mTaskHandle);
}

void * Task::GetContext()
{
    return mContext;
    //     return xTaskGetAffinity(mTaskHandle);
}
//     }
// }
