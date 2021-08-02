#ifndef PROVISION_H_
#define PROVISION_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "indhilib.h"

typedef enum
{
    INDHI_LIB_PROVISIONING_NONE = 0,
    INDHI_LIB_PROVISIONING_INIT,
    INDHI_LIB_PROVISIONING_SUCCESS,
    INDHI_LIB_PROVISIONING_FAIL,
} indhilib_status_t;

typedef enum
{
    ONBOARDING_MODE_NONE = 0,
    ONBOARDING_MODE_WIFI,
    ONBOARDING_MODE_BLE,
    ONBOARDING_MODE_MAX,
} onboarding_mode_t;

ErrorCode_t indhilib_provisioning_init(void);
ErrorCode_t indhilib_provisioning_deinit(void);
indhilib_status_t indhilib_provisioning_get_success_status(int wait_ms = -1);
ErrorCode_t indhilib_provisioning_set_success_status(void);
ErrorCode_t indhilib_provisioning_clear_success_status(void);
ErrorCode_t indhilib_provisioning_clear_failed_status(void);
ErrorCode_t indhilib_provisioning_set_failed_status(void);
ErrorCode_t indhilib_provisioning_set_validation_endpoint(char *validation_endp);
char *indhilib_provisioning_get_validation_endpoint(void);
char *indhilib_provisioning_get_validation_token(void);
ErrorCode_t indhilib_provisioning_set_validation_token(char *validation_tokn);
ErrorCode_t indhilib_provisioning_set_api_key(char *api_ky);
char *indhilib_provisioning_get_api_key(void);
char *indhilib_provisioning_get_initialization_endpoint(void);
ErrorCode_t indhilib_provisioning_set_initialization_endpoint(char *init_endp);
char *indhilib_provisioning_get_init_request(void);

ErrorCode_t indhilib_provisioning(void);
ErrorCode_t indhilib_provisioning(char *token);
char *indhilib_device_init_create_request();
ErrorCode_t indhilib_device_init_fetch_request();

ErrorCode_t indhilib_validation_token_save(void);
ErrorCode_t indhilib_validation_endpoint_save(void);
ErrorCode_t indhilib_initialization_endpoint_save(void);
ErrorCode_t indhilib_api_key_save(void);
ErrorCode_t indhilib_set_router_ssid(char *__ssid);
ErrorCode_t indhilib_set_router_psk(char *__psk);
ErrorCode_t indhilib_set_router_credential(char *__ssid, char *__psk);
ErrorCode_t indhilib_set_ap_ssid(char *__ssid);
ErrorCode_t indhilib_set_ap_psk(char *__psk);
ErrorCode_t indhilib_set_ap_credential(char *__ssid, char *__psk);
char *indhilib_get_ap_ssid(void);
char *indhilib_get_router_ssid(void);
char *indhilib_get_router_psk(void);

ErrorCode_t indhilib_enable_wifi_onboarding(void);
ErrorCode_t indhilib_disable_wifi_onboarding(void);

#endif