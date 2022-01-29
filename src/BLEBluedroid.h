#ifndef BLE_BLUEDROID_H_
#define BLE_BLUEDROID_H_

#include "esp_bt_main.h"

/**
 * @brief Static class used to call the Bluebird library API's
 *
 */
class BLEBluedroid
{
private:

public:
    /**
     * @brief Initialize the bluebird
     *
     */
    static void Init(void);
    /**
     * @brief Deinitialize the bluebird
     *
     */
    static void Deinit(void);
    /**
     * @brief Enable the bluebird
     *
     */
    static void Enable(void);
    /**
     * @brief Disable the bluebird
     *
     */
    static void Disable(void);
    /**
     * @brief Get the Status object of bluebird
     *
     * @return esp_bluedroid_status_t status of bluebird
     */
    static esp_bluedroid_status_t GetStatus(void);
};


#endif
