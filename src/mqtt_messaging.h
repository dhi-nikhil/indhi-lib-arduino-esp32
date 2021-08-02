#ifndef MQTT_MESSAGING_H_
#define MQTT_MESSAGING_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"

ErrorCode_t mqtt_connection_init(void);
ErrorCode_t mqtt_connection_deinit(void);
// indhilib_status_t mqtt_connection_get_success_status(int wait_ms);
// ErrorCode_t mqtt_connection_set_success_status(void);
// ErrorCode_t mqtt_connection_clear_success_status(void);
ErrorCode_t mqtt_connection_set_mqtt_uri(char *uri);
char *mqtt_connection_get_mqtt_uri(void);
ErrorCode_t mqtt_connection_set_mqtt_client_id(char *client_id);
char *mqtt_connection_get_mqtt_client_id(void);
ErrorCode_t mqtt_connection_set_device_subscription_topic(char *client_id);
char *mqtt_connection_get_device_subscription_topic(void);
ErrorCode_t mqtt_connection_set_device_publishing_topic(char *client_id);
char *mqtt_connection_get_device_publishing_topic(void);
int mqtt_connection_get_mqtt_connection_verification_required(void);
ErrorCode_t mqtt_connection_set_mqtt_connection_verification_required(void);
ErrorCode_t mqtt_connection_clear_mqtt_connection_verification_required(void);
ErrorCode_t mqtt_connection_send_data(char *topic, char *data, int qos);
ErrorCode_t mqtt_connection_start(void);
ErrorCode_t mqtt_connection_stop(void);
#endif