#ifndef MQTT_STATUS_MANAGER_H_
#define MQTT_STATUS_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "event_manager.h"

typedef int mqtt_status_t;

ErrorCode_t mqtt_event_init(void);
ErrorCode_t mqtt_event_deinit(void);
mqtt_status_t mqtt_event_connected_get_status(int wait_ms = 0);
ErrorCode_t mqtt_event_connected_set_status(void);
ErrorCode_t mqtt_event_connected_clear_status(void);
mqtt_status_t mqtt_event_activation_get_status(int wait_ms);
ErrorCode_t mqtt_event_activation_set_status(void);
ErrorCode_t mqtt_event_activation_clear_status(void);
ErrorCode_t mqtt_event_deconfigure_set_status(void);
ErrorCode_t mqtt_event_deconfigure_clear_status(void);

#endif