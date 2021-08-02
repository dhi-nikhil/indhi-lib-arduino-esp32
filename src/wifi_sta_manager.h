#ifndef WIFI_STA_MANAGER_H_
#define WIFI_STA_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "log_handler.h"
#include "error_code_manager.h"
#include "nvs_manager.h"
#include "wifi_manager.h"
#include "network_manager.h"
#include "event_manager.h"
#include "esp_wifi_types.h"
#include "wifi_status_manager.h"

#ifndef CONFIG_MAX_AP_SCAN_RECORDS
#define CONFIG_MAX_AP_SCAN_RECORDS 10
#endif

ErrorCode_t wifi_sta_scan(bool synchronous = true);
wifi_scan_status_t wifi_scanning_get_status(void);
ErrorCode_t wifi_scanning_set_status(wifi_scan_status_t wifi_scn_status);
ErrorCode_t wifi_sta_ssid_set(const char *ssid, const int length);
ErrorCode_t wifi_sta_password_set(const char *password, const int length);
ErrorCode_t wifi_sta_connect(void);
ErrorCode_t wifi_sta_disconnect(void);
// ErrorCode_t wifi_sta_single_entry_json_object(cJSON **object, char *ssid, char *wifi_auth, int rssi);
char *wifi_auth_mode_to_string(wifi_auth_mode_t mode);
ErrorCode_t wifi_sta_scan_results(char **scan_result);
ErrorCode_t wifi_sta_config(void);
ErrorCode_t wifi_sta_credential_save(void);
ErrorCode_t wifi_sta_credential_retrive(void);
ErrorCode_t wifi_sta_config(char *ssid, char *password);
ErrorCode_t wifi_sta_start(void);
ErrorCode_t wifi_sta_stop(void);
ErrorCode_t wifi_sta_init(void);
ErrorCode_t wifi_sta_deinit(void);

#endif