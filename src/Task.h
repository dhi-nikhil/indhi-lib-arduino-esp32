#ifndef FREERTOS_TASK_H_
#define FREERTOS_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string"
#include "Task.h"
#include "esp_log.h"
#include "sdkconfig.h"

class Task
{
private:
    xTaskHandle mTaskHandle;
    eTaskState mTaskState;
    void * mTaskData;
    std::string mTaskName;
    uint16_t mStaskSize;
    uint8_t mPriority;
    BaseType_t mCoreId;
    TaskFunction_t mTaskFunction;
    void * mContext;

public:
    Task();
    Task(std::string name, uint16_t stackSize, uint8_t priority, TaskFunction_t taskFunction, void * context);
    ~Task();
    Task * SetStackSize(uint16_t stackSize);
    Task * SetPriority(uint8_t priority);
    Task * SetName(std::string name);
    Task * SetCore(BaseType_t coreId);
    Task * SetTaskFunction(TaskFunction_t taskFunction);
    Task * SetContext(void * context);
    void * GetContext(void * context);
    TaskFunction_t GetTaskFunction(TaskFunction_t taskFunction);
    uint16_t GetStackSize(void);
    UBaseType_t getPriority(void);
    std::string GetName(void);
    BaseType_t GetCore();
    void * GetContext();
    void Start(void * taskData = nullptr);
    void Delete();
    // virtual void run(void *data) = 0;
    static void Delay(int milliSeconds);
    void Suspend();
    void Resume();
    TickType_t GetTickCount();
    std::string GetTaskName();
    // BaseType_t GetCore();
    void ResumeFromISR();
    eTaskState GetState();
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    void Describe();
#endif
    TaskHandle_t getHandle() const;
};

#endif
