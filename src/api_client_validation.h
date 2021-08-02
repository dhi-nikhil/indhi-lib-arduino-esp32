#ifndef API_CLIENT_VALIDATION_H_
#define API_CLIENT_VALIDATION_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"

ErrorCode_t api_client_validation_init(void);
ErrorCode_t api_client_validation(void);
ErrorCode_t api_client_validation_deinit(void);

#endif