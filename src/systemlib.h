#ifndef SYSTEMLIB_H_
#define SYSTEMLIB_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "log_handler.h"
#include "error_code_manager.h"
#include "nvs_manager.h"
#include "network_manager.h"
#include "event_manager.h"
#include "esp_wifi_types.h"

int is_hex(int x);
int url_decode(const char *s, char *dec);
void url_encode(char *s, char *dec);
ErrorCode_t hex_to_string(char *__string, uint8_t *__hex, size_t size);

#endif