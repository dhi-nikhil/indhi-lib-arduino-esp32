#ifndef API_RESPONSE_PARSER_H_
#define API_RESPONSE_PARSER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "cJSON.h"

ErrorCode_t api_response_data_parser(char *raw_data, int *code, char *message, cJSON **data_j);
ErrorCode_t api_response_api_key_parser(char *data, char *api_key);
ErrorCode_t api_response_mqtt_uri_parser(char *data, char *mqtt_uri);
ErrorCode_t api_response_mqtt_username_parser(char *data, char *mqtt_username);
ErrorCode_t api_response_mqtt_password_parser(char *data, char *mqtt_password);
ErrorCode_t api_response_mqtt_client_id_parser(char *data, char *mqtt_client_id);
ErrorCode_t api_response_mqtt_primary_subscription_topic_parser(char *data, char *mqtt_device_primary_subscription_topic);
ErrorCode_t api_response_mqtt_primary_publishing_topic_parser(char *data, char *mqtt_device_primary_publishing_topic);
ErrorCode_t api_response_mqtt_connection_verification_parser(char *data, int *mqtt_connection_verification);
ErrorCode_t api_response_feature_topics_parse(char *data);
ErrorCode_t api_response_feature_count_parse(char *data, int *feature_count);
ErrorCode_t api_response_feature_details_parse(char *data, int id, char *feature_id, char *subscription_topic, char *publishing_topic);

#endif