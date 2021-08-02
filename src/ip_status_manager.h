#ifndef IP_STATUS_MANAGER_H_
#define IP_STATUS_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "event_manager.h"

typedef int ip_status_t;

ErrorCode_t ip_event_init(void);
ErrorCode_t ip_event_deinit(void);
ip_status_t ip_event_get_status(int wait_ms = 0);
ErrorCode_t ip_event_set_status(void);
ErrorCode_t ip_event_clear_status(void);

#endif