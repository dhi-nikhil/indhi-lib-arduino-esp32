#ifndef DEBUG_LOG_H_
#define DEBUG_LOG_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "log_handler.h"

#define print_free_heap logw(__func__, "free memory : %d; Line : %d", esp_get_free_heap_size(), __LINE__)

#endif