#ifndef FREERTOS_TASK_H_
#define FREERTOS_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string"
#include "Task.h"
#include "esp_log.h"
#include "sdkconfig.h"

/**
 * @brief Task class
 *
 */
class Task
{
private:
    xTaskHandle mTaskHandle; /* task handle */
    eTaskState mTaskState; /* task state */
    void * mTaskData; /* task data */
    std::string mTaskName; /* task name */
    uint16_t mStaskSize; /* task stack size */
    uint8_t mPriority; /* task priority */
    BaseType_t mCoreId; /* task core id */
    TaskFunction_t mTaskFunction; /* task function */
    void * mContext; /* task context */

public:
    /**
     * @brief Construct a new Task object
     *
     */
    Task();
    /**
     * @brief Construct a new Task object
     *
     * @param name name of the task
     * @param stackSize size of the task
     * @param priority priority of the task
     * @param taskFunction task function
     * @param context task context
     */
    Task(std::string name, uint16_t stackSize, uint8_t priority, TaskFunction_t taskFunction, void * context);
    /**
     * @brief Destroy the Task object
     *
     */
    ~Task();
    /**
     * @brief Set the Stack Size object
     *
     * @param stackSize
     * @return Task*
     */
    Task * SetStackSize(uint16_t stackSize);
    /**
     * @brief Set the Priority object
     *
     * @param priority
     * @return Task*
     */
    Task * SetPriority(uint8_t priority);
    /**
     * @brief Set the Name object
     *
     * @param name
     * @return Task*
     */
    Task * SetName(std::string name);
    /**
     * @brief Set the Core object
     *
     * @param coreId
     * @return Task*
     */
    Task * SetCore(BaseType_t coreId);
    /**
     * @brief Set the Task Function object
     *
     * @param taskFunction
     * @return Task*
     */
    Task * SetTaskFunction(TaskFunction_t taskFunction);
    /**
     * @brief Set the Context object
     *
     * @param context
     * @return Task*
     */
    Task * SetContext(void * context);
    /**
     * @brief Get the Context object
     *
     * @param context
     * @return void*
     */
    void * GetContext(void * context);
    /**
     * @brief Get the Task Function object
     *
     * @param taskFunction
     * @return TaskFunction_t
     */
    TaskFunction_t GetTaskFunction(TaskFunction_t taskFunction);
    /**
     * @brief Get the Stack Size object
     *
     * @return uint16_t return task size
     */
    uint16_t GetStackSize(void);
    /**
     * @brief Get the Priority object
     *
     * @return UBaseType_t task priority
     */
    UBaseType_t getPriority(void);
    /**
     * @brief Get the Name object
     *
     * @return std::string task name
     */
    std::string GetName(void);
    /**
     * @brief Get the Core object
     *
     * @return BaseType_t
     */
    BaseType_t GetCore();
    /**
     * @brief Get the Context object
     *
     * @return void*
     */
    void * GetContext();
    /**
     * @brief Start the task
     *
     * @param taskData
     */
    void Start(void * taskData = nullptr);
    /**
     * @brief Delete the task
     *
     */
    void Delete();
    /**
     * @brief Delay for the task
     *
     * @param milliSeconds
     */
    static void Delay(int milliSeconds);
    /**
     * @brief Suspend task
     *
     */
    void Suspend();
    /**
     * @brief Resume task
     *
     */
    void Resume();
    /**
     * @brief Get the Tick Count object
     *
     * @return TickType_t
     */
    TickType_t GetTickCount();
    /**
     * @brief Get the Task Name object
     *
     * @return std::string
     */
    std::string GetTaskName();
    // BaseType_t GetCore();
    /**
     * @brief Resume task from ISR
     *
     */
    void ResumeFromISR();
    /**
     * @brief Get the State object
     *
     * @return eTaskState
     */
    eTaskState GetState();
#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    /**
     * @brief Describe the task
     *
     */
    void Describe();
#endif
    /**
     * @brief Get the Handle object
     *
     * @return TaskHandle_t
     */
    TaskHandle_t getHandle() const;
};

#endif
