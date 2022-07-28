#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "NVStorage.h"
#include "esp_log.h"
#include <string>

static const char * TAG = "NVStorage";

// namespace Idf
// {
//     namespace Nvs
//     {
NVStorage::NVStorage(std::string name, nvs_open_mode_t openMode)
{
    ESP_LOGI(TAG, "initialized");
    esp_err_t esp_err = Init();
    if (esp_err != ESP_OK)
    {
        ESP_LOGE(TAG, "NVS Flash init failed");
    }
    mName = name;
    Open(name, openMode);
}

esp_err_t NVStorage::Init()
{
    ESP_LOGI(TAG, "NVS Flash initialized with static method");
    return ::nvs_flash_init();
}

esp_err_t NVStorage::Deinit()
{
    ESP_LOGI(TAG, "NVS Flash deinitialized with static method");
    return ::nvs_flash_deinit();
}

NVStorage::NVStorage()
{
    ESP_LOGI(TAG, "NVStorage initialized with Constructor");
    ESP_ERROR_CHECK_WITHOUT_ABORT(Init());
}

NVStorage::~NVStorage()
{
    ESP_LOGI(TAG, "NVStorage deinitialized with Distructor");
    ESP_ERROR_CHECK_WITHOUT_ABORT(Deinit());
}

esp_err_t NVStorage::Open(std::string name, nvs_open_mode_t openMode)
{
    return ::nvs_open(name.c_str(), openMode, &mHandle);
}

void NVStorage::Close()
{
    if (mHandle)
        ::nvs_close(mHandle);
}

esp_err_t NVStorage::Commit()
{
    return ::nvs_commit(mHandle);
}

esp_err_t NVStorage::Erase()
{
    return ::nvs_erase_all(mHandle);
}

esp_err_t NVStorage::Erase(std::string key)
{
    esp_err_t err = ESP_OK;
    err = ::nvs_erase_key(mHandle, key.c_str());
    if (err != ESP_OK)
    {
        return err;
    }

    return Commit();
}

esp_err_t NVStorage::Get(std::string key, std::string * result)
{
    size_t length;
    esp_err_t esp_err;
    esp_err = ::nvs_get_str(mHandle, key.c_str(), NULL, &length);
    if (esp_err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error getting key %s : %i (0x%x)", key.c_str(), esp_err, esp_err);
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_err);
        return ESP_FAIL;
    }

    char * data = (char *) calloc(length, sizeof(char));
    ::nvs_get_str(mHandle, key.c_str(), data, &length);
    *result = std::string(data, length);
    free(data);
    return ESP_OK;
}

esp_err_t NVStorage::Set(std::string key, std::string data)
{
    esp_err_t err;
    err = ::nvs_set_str(mHandle, key.c_str(), data.c_str());
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    return err;
}

esp_err_t NVStorage::Get(std::string key, uint8_t * result, size_t & length)
{
    return ::nvs_get_blob(mHandle, key.c_str(), result, &length);
}

esp_err_t NVStorage::Set(std::string key, uint8_t * data, size_t length)
{
    return ::nvs_set_blob(mHandle, key.c_str(), data, length);
}

esp_err_t NVStorage::Get(std::string key, uint32_t & value)
{
    return ::nvs_get_u32(mHandle, key.c_str(), &value);
}

esp_err_t NVStorage::Set(std::string key, uint32_t value)
{
    return ::nvs_set_u32(mHandle, key.c_str(), value);
}

//     }
// }
