#ifndef NVS_MANAGER_H_
#define NVS_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"

ErrorCode_t non_volatile_storage_init(void);
ErrorCode_t non_volatile_storage_deinit(void);
ErrorCode_t non_volatile_storage_save_value(char *key, char *value);
ErrorCode_t non_volatile_storage_erase_value(char *key);
ErrorCode_t non_volatile_storage_retrive_value(char *key, char *value);
ErrorCode_t non_volatile_storage_open(void);
ErrorCode_t non_volatile_storage_erase_data(void);

#endif