#define LOG_LOCAL_LEVEL ESP_LOG_NONE

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
#include "NetworkInterface.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char * TAG = "NetworkInterface";

NetworkInterface::NetworkInterface()
{
    ESP_LOGI(TAG, "Network interface init with Constructor");
    esp_netif_init();
}

NetworkInterface::~NetworkInterface()
{
    ESP_LOGI(TAG, "Network interface deinit with Distructor");
    esp_netif_deinit();
}

esp_err_t NetworkInterface::Init()
{
    ESP_LOGI(TAG, "Network interface init with static method");
    return ::esp_netif_init();
}

esp_err_t NetworkInterface::Deinit()
{
    ESP_LOGI(TAG, "Network interface deinit with static method");
    return ::esp_netif_deinit();
}

