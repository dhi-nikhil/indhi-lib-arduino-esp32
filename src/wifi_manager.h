#ifndef WIFI_MANAGER_H_
#define WIFI_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "esp_wifi.h"

ErrorCode_t wifi_init(void);
ErrorCode_t wifi_deinit(void);
ErrorCode_t wifi_start(void);
ErrorCode_t wifi_stop(void);
ErrorCode_t wifi_set_mode_to_ap(void);
ErrorCode_t wifi_set_mode_to_sta(void);
ErrorCode_t wifi_set_mode_to_apsta(void);
ErrorCode_t wifi_set_ap_config(wifi_config_t *config);
ErrorCode_t wifi_update_mode_to_ap(void);
ErrorCode_t wifi_update_mode_to_sta(void);
ErrorCode_t wifi_clear_mode(wifi_mode_t wifi_mode);
ErrorCode_t wifi_clear_mode_ap(void);
ErrorCode_t wifi_clear_mode_sta(void);
ErrorCode_t wifi_clear_mode_apsta(void);
ErrorCode_t wifi_get_mode(void);

#endif