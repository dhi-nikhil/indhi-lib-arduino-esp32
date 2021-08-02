#ifndef API_CLIENT_DEVICE_INIT_H_
#define API_CLIENT_DEVICE_INIT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"

ErrorCode_t api_client_device_init_init(void);
ErrorCode_t api_client_device_init(void);
ErrorCode_t api_client_device_init_deinit(void);

#endif