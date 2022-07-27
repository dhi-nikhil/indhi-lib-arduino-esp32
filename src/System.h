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
    /**
     * @brief Construct a new System object
     *
     */
    System();
    /**
     * @brief Destroy the System object
     *
     */
    ~System();
    /**
     * @brief Pin mapping
     *
     */
    static void dumpPinMapping(); // Dump the mappings of pins to functions.
    /**
     * @brief Heap info
     *
     */
    static void dumpHeapInfo();
    /**
     * @brief Get the Chip Info object
     *
     * @param info info of the chip
     */
    static void getChipInfo(esp_chip_info_t * info);
    /**
     * @brief Get the Free Heap Size object
     *
     * @return size_t size of free heap memory
     */
    static size_t getFreeHeapSize();
    /**
     * @brief Get the IDF version
     *
     * @return std::string return the string of rfid tag
     */
    static std::string getIDFVersion();
    /**
     * @brief Get the Minimum Free Heap Size object
     *
     * @return size_t return the minimum heap memory
     */
    static size_t getMinimumFreeHeapSize();
    /**
     * @brief Print minimum free heap memory
     *
     */
    static void PrintMinimumFreeHeapSize();
    /**
     * @brief Restart the controller
     *
     */
    static void restart();
    /**
     * @brief Get the Number Of Tasks object
     *
     * @return int number of tasks
     */
    static int getNumberOfTasks();
    /**
     * @brief Set the Mac Address object
     *
     * @param mac MAC ID to set
     */
    static void setMacAddress(std::string mac);
};
// }

#endif
