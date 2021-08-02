#ifndef NETWORK_MANAGER_H_
#define NETWORK_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"

ErrorCode_t network_init(void);
ErrorCode_t network_deinit(void);

#endif