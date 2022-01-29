#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdint.h>
#include <string>
#include <iomanip>
#include <string>   // std::string
#include <iostream> // std::cout
#include <sstream> // std::stringstream

// namespace FreeRTOS
// {
//     namespace Semaphore
//     {
typedef enum
{
    SEMAPHORE_TYPE_NONE,
    SEMAPHORE_TYPE_BINARY,
    SEMAPHORE_TYPE_COUNTING,
    SEMAPHORE_TYPE_MAX,
} SemaphoreType_t;

class BaseSemaphore
{
protected:
    xSemaphoreHandle mSemaphoreHandle;
    std::string mName;
    std::string mOwner;
    uint32_t mValue;

public:
    BaseSemaphore(std::string name);
    BaseSemaphore();
    ~BaseSemaphore();
    UBaseType_t GetCount();
    void Give();
    void Give(uint32_t value);
    void giveFromISR();
    void takeFromISR();
    void SetName(std::string name);
    bool Take(std::string owner = "<Unknown>");
    bool Take(uint32_t timeout, std::string owner = "<Unknown>");
    std::string ToString();
    uint32_t Wait(std::string owner = "<Unknown>");
    bool TimedWait(std::string owner = "<Unknown>", uint32_t timeout = portMAX_DELAY);
    uint32_t Value() { return mValue; };
};

class BinarySemaphore : public BaseSemaphore
{
    // private:
public:
    BinarySemaphore(std::string owner = "<Unknown>");
    ~BinarySemaphore();
};

class CountingSemaphore : public BaseSemaphore
{
private:
    int mCount;

public:
    CountingSemaphore(std::string owner = "<Unknown>", int count = 3);
    ~CountingSemaphore();
};

class Mutex : public BaseSemaphore
{
private:
public:
    Mutex(std::string owner = "<Unknown>");
    ~Mutex();
};

class RecursiveMutex : public BaseSemaphore
{
private:
public:
    RecursiveMutex(std::string owner = "<Unknown>");
    ~RecursiveMutex();
    void giveRecursive();
    bool takeRecursive(std::string owner = "<Unknown>");
};
//     }
// }

#endif
