#ifndef TIMERM_H_
#define TIMERM_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "string"

// namespace FreeRTOS
// {
    // namespace Timer
    // {
        class Timer
        {
        private:
            std::string mName;
            TimerHandle_t mTimerHandle;
            void (*mCallback)(Timer *pTimer);
            static void InternalCallback(TimerHandle_t timerHandle);

        public:
            Timer(std::string name, TickType_t period, UBaseType_t reload, void *data, void (*callback)(Timer *pTimer));
            ~Timer();
            void ChangePeriod(TickType_t period, TickType_t blockTime = portMAX_DELAY);
            void *GetData();
            std::string GetName();
            TickType_t GetPeriod();
            void reset(TickType_t blockTime = portMAX_DELAY);
            void start(TickType_t blockTime = portMAX_DELAY);
            void stop(TickType_t blockTime = portMAX_DELAY);
        };
//     }
// }

#endif
