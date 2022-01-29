#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>
#include <string>
#include <esp_system.h>

// namespace System
// {
class System
{
public:
    System();
    ~System();
    static void dumpPinMapping(); // Dump the mappings of pins to functions.
    static void dumpHeapInfo();
    static void getChipInfo(esp_chip_info_t * info);
    static size_t getFreeHeapSize();
    static std::string getIDFVersion();
    static size_t getMinimumFreeHeapSize();
    static void PrintMinimumFreeHeapSize();
    static void restart();
    static int getNumberOfTasks();
    static void setMacAddress(std::string mac);
};
// }

#endif
