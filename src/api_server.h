#ifndef API_SERVER_H_
#define API_SERVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "log_handler.h"
#include "error_code_manager.h"
#include "nvs_manager.h"
#include "network_manager.h"
#include "event_manager.h"
#include "wifi_manager.h"
#include "wifi_ap_manager.h"
#include "esp_http_server.h"

ErrorCode_t api_server_start(void);
ErrorCode_t api_server_stop(void);
ErrorCode_t api_server_init(void);
ErrorCode_t api_server_deinit(void);
ErrorCode_t api_server_json_response_send(httpd_req_t *r, const char *status, uint16_t code, char *message, char *data, bool success);

#endif