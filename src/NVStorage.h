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
class NVStorage
{
private:
    std::string mName;
    nvs_handle_t mHandle;

public:
    NVStorage(std::string name, nvs_open_mode_t openMode = NVS_READWRITE);
    NVStorage();
    ~NVStorage();
    static esp_err_t Init();
    static esp_err_t Deinit();
    void Open(std::string name = "default", nvs_open_mode_t openMode = NVS_READWRITE);
    void Close();
    esp_err_t Commit();
    void Erase();
    void Erase(std::string key);
    esp_err_t Get(std::string key, std::string * result);
    esp_err_t Set(std::string key, std::string data);
    esp_err_t Get(std::string key, uint8_t * result, size_t & length);
    esp_err_t Set(std::string key, uint8_t * data, size_t length);
    esp_err_t Get(std::string key, uint32_t & value);
    esp_err_t Set(std::string key, uint32_t value);
};
//     }
// }

#endif
