#ifndef LOG_HANDLER_H_
#define LOG_HANDLER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define loge(tag, format, ...)                                          \
    {                                                                   \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, tag, format, ##__VA_ARGS__); \
    }
#define logw(tag, format, ...)                                         \
    {                                                                  \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, tag, format, ##__VA_ARGS__); \
    }
#define logi(tag, format, ...)                                         \
    {                                                                  \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, tag, format, ##__VA_ARGS__); \
    }
#define logd(tag, format, ...)                                          \
    {                                                                   \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_DEBUG, tag, format, ##__VA_ARGS__); \
    }
#define logv(tag, format, ...)                                            \
    {                                                                     \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_VERBOSE, tag, format, ##__VA_ARGS__); \
    }

#define logfe(format, ...)                                                   \
    {                                                                        \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, __func__, format, ##__VA_ARGS__); \
    }
#define logfw(format, ...)                                                  \
    {                                                                       \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, __func__, format, ##__VA_ARGS__); \
    }
#define logfi(format, ...)                                                  \
    {                                                                       \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, __func__, format, ##__VA_ARGS__); \
    }
#define logfd(format, ...)                                                   \
    {                                                                        \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_DEBUG, __func__, format, ##__VA_ARGS__); \
    }
#define logfv(format, ...)                                                     \
    {                                                                          \
        ESP_LOG_LEVEL_LOCAL(ESP_LOG_VERBOSE, __func__, format, ##__VA_ARGS__); \
    }

#endif