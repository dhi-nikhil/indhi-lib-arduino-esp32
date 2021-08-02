#ifndef WIFI_AP_MANAGER_H_
#define WIFI_AP_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "log_handler.h"
#include "error_code_manager.h"
#include "nvs_manager.h"
#include "wifi_manager.h"
#include "network_manager.h"
#include "event_manager.h"
#include "esp_wifi_types.h"

ErrorCode_t wifi_ap_ip_modify(void);
ErrorCode_t wifi_ap_init(void);
ErrorCode_t wifi_ap_config(char *ssid, char *password, uint8_t wifi_auth_mode);
ErrorCode_t wifi_ap_start(void);
ErrorCode_t wifi_ap_stop(void);
ErrorCode_t wifi_ap_deinit(void);

#endif