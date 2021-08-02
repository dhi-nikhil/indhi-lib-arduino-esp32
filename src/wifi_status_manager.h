#ifndef WIFI_STATUS_MANAGER_H_
#define WIFI_STATUS_MANAGER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "event_manager.h"

typedef int wifi_status_t;

typedef enum
{
    WIFI_STATE_IDLE = 0,
    WIFI_STATE_SCAN,
    WIFI_STATE_SCANNING,
    WIFI_STATE_SCANNED,
    WIFI_STATE_ERROR,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_DISCONNECTED,
    WIFI_STATE_INVALID,
    WIFI_STATE_CONNECTING,
} wifi_scan_status_t;

ErrorCode_t wifi_event_init(void);
ErrorCode_t wifi_set_status(int state);
ErrorCode_t wifi_event_connection_set_status(int wifi_connecting_status);
void wifi_status_manager_set_status(int status);
ErrorCode_t wifi_event_deinit(void);
wifi_status_t wifi_event_connected_get_status(int wait_ms = 0);
wifi_status_t wifi_event_connecting_get_status(void);
wifi_status_t wifi_status_manager_get_status(void);

#endif