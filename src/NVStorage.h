#ifndef NV_STORAGE_H_
#define NV_STORAGE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string>

// namespace Idf
// {
//     namespace Nvs
//     {
/**
 * @brief NVStorage class
 *
 */
class NVStorage
{
private:
    std::string mName; /* NVStorage name */
    nvs_handle_t mHandle; /* NVStorage handle */

public:
    /**
     * @brief Construct a new NVStorage object
     *
     * @param name name of the storage
     * @param openMode mode to open the storage
     */
    NVStorage(std::string name, nvs_open_mode_t openMode = NVS_READWRITE);
    /**
     * @brief Construct a new NVStorage object
     *
     */
    NVStorage();
    /**
     * @brief Destroy the NVStorage object
     *
     */
    ~NVStorage();
    /**
     * @brief Static method to Initialize the MQTT
     *
     * @return esp_err_t
     */
    static esp_err_t Init();
    /**
     * @brief Static method to deinitialize the MQTT
     *
     * @return esp_err_t
     */
    static esp_err_t Deinit();
    /**
     * @brief Open the NVStoragege
     *
     * @param name name of the storage
     * @param openMode mode to open the storage
     * @return esp_err_t return if success or failed
     */
    esp_err_t Open(std::string name = "default", nvs_open_mode_t openMode = NVS_READWRITE);
    /**
     * @brief Close the NVStorage
     *
     */
    void Close();
    /**
     * @brief Commit the data to storage
     *
     * @return esp_err_t
     */
    esp_err_t Commit();
    /**
     * @brief Erase all data from nvs
     *
     * @return esp_err_t return if success or failed
     */
    esp_err_t Erase();
    /**
     * @brief Erase the data with key
     *
     * @param key key to erase from storage
     * @return esp_err_t return if success or failed
     */
    esp_err_t Erase(std::string key);
    /**
     * @brief Get the data from storage
     *
     * @param key key to get
     * @param result value fetched
     * @return esp_err_t return if success or failed
     */
    esp_err_t Get(std::string key, std::string * result);
    /**
     * @brief Set the data in the storage
     *
     * @param key set the key
     * @param data set the value
     * @return esp_err_t return if success or failed
     */
    esp_err_t Set(std::string key, std::string data);
    /**
     * @brief Get the data from storage
     *
     * @param key Key to fetched data
     * @param result fetched data
     * @param length length of the data
     * @return esp_err_t return if success or failed
     */
    esp_err_t Get(std::string key, uint8_t * result, size_t & length);
    /**
     * @brief Set the data in the storage
     *
     * @param key set the key
     * @param data set the value
     * @param length length of the data
     * @return esp_err_t return if success or failed
     */
    esp_err_t Set(std::string key, uint8_t * data, size_t length);
    /**
     * @brief Get the data from the storage
     *
     * @param key key to fetch data
     * @param value fetched data
     * @return esp_err_t return if success or failed
     */
    esp_err_t Get(std::string key, uint32_t & value);
    /**
     * @brief Set the data in the storage
     *
     * @param key set the key
     * @param value set the value
     * @return esp_err_t return if success or failed
     */
    esp_err_t Set(std::string key, uint32_t value);
};
//     }
// }

#endif
