#ifndef BT_CONTROLLER_H_
#define BT_CONTROLLER_H_

#include "esp_bt.h"

/**
 * @brief Class to control the BLE Controller
 *
 */
class BTController
{
private:
    /**
     * @brief Static structure variable to store the controller configuration
     *
     */
    static esp_bt_controller_config_t mBTControllerConfig;
public:
    /**
     * @brief Configure Bluedroid
     *
     */
    static void Config(void);
    /**
     * @brief Initialize Bluedroid
     *
     */
    static void Init(void);
    /**
     * @brief Deinitialize Bluedroid
     *
     */
    static void Deinit(void);
    /**
     * @brief Enable Bluedroid
     *
     * @param bluetoothMode specify esp_bt_mode_t
     */
    static void Enable(esp_bt_mode_t bluetoothMode);
    /**
     * @brief Disable Bluedroid
     *
     */
    static void Disable(void);
    /**
     * @brief Get the Status of Bluetooth controller
     *
     * @return esp_bt_controller_status_t status of bluetooth controller
     */
    static esp_bt_controller_status_t GetStatus(void);
    /**
     * @brief Release the Bluetooth controller memory
     *
     * @param bluetoothMode from where to release the memory
     */
    static void BTControllerMemoryRelease(esp_bt_mode_t bluetoothMode);
    /**
     * @brief Release the Bluetooth memory
     *
     * @param bluetoothMode
     */
    static void BTMemoryRelease(esp_bt_mode_t bluetoothMode);
    /**
     * @brief Enable the sleep mode
     *
     */
    static void SleepModeEnable(void);
    /**
     * @brief Disable the sleep mode
     *
     */
    static void SleepModeDisable(void);

};

#endif
