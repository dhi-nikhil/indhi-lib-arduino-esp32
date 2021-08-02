#ifndef PING_IP4_H_
#define PING_IP4_H_

#include "error_code_manager.h"

typedef enum
{
    PING_STATUS_IDLE = 0,
    PING_STATUS_FAILED,
    PING_STATUS_SUCCESS,
    PING_STATUS_INVALID,
    PING_STATUS_PROCESSING,
    PING_STATUS_PROCESSED,
    PING_STATUS_TIMEOUT,
} ping_status_t;

ErrorCode_t __ping_deinit(void);
ErrorCode_t __ping_init(void);
ErrorCode_t ping_ipv4(char *ip_address, int wait_ms = 6000);
ErrorCode_t ping_status_init(void);
ErrorCode_t ping_status_deinit(void);
ping_status_t ping_status_get(int wait_ms = 0);
ping_status_t ping_status_manager_get_status(void);

#endif