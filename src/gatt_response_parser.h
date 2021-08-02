#ifndef GATT_RESPONSE_PARSER_H_
#define GATT_RESPONSE_PARSER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "cJSON.h"

ErrorCode_t gatt_data_parser(char *raw_data, cJSON **data_j);
ErrorCode_t gatt_validation_endpoint_parser(char *data, char *validation_endpoint);
ErrorCode_t gatt_initialization_endpoint_parser(char *data, char *initialization_endpoint);
ErrorCode_t gatt_validation_token_parser(char *data, char *validation_token);
ErrorCode_t gatt_server_ip_address_parser(char *data, char *server_ip_address);
ErrorCode_t gatt_sta_password_parser(char *data, char *sta_password);
ErrorCode_t gatt_sta_ssid_parser(char *data, char *sta_ssid);

#endif