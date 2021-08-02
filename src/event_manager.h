#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"

ErrorCode_t event_manager_init(void);
ErrorCode_t event_manager_deinit(void);
ErrorCode_t event_manager_register_wifi_event(void);
ErrorCode_t event_manager_unregister_wifi_event(void);
ErrorCode_t event_manager_register_ip_event(void);
ErrorCode_t event_manager_unregister_ip_event(void);
ErrorCode_t event_manager_register_mqtt_event(void);
ErrorCode_t event_manager_unregister_mqtt_event(void);

#endif