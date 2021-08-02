#ifndef MQTT_STATIC_MESSAGES_H_
#define MQTT_STATIC_MESSAGES_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "helper.h"
#include "log_handler.h"
#include "cJSON.h"
#include "string.h"

#define ACTIVATION_SUCCESS "{\"command_type\":\"activation\",\"status\":\"success\"}"
#define ACTIVATION_FAILED "{\"command_type\":\"activation\",\"status\":\"failed\"}"
#define ACTIVATION_PENDING "{\"command_type\":\"activation\",\"status\":\"pending\"}"
#define ACTIVATION_TIMEOUT "{\"command_type\":\"activation\",\"status\":\"timeout\"}"
#define ACTIVATION_DECONFIGURE "{\"command_type\":\"activation\",\"status\":\"deconfigure\"}"
#define MQTT_CONNECTED "{\"command_type\":\"connected\"}"
#define MQTT_DISCONNECTED "{\"command_type\":\"disconnected\"}"
#define ADDITION_CONFIRMATION "{\"command_type\":\"additionConfirmation\"}"
#define DECONFIGURE_MESSAGE "{\"command_type\":\"deconfigure\"}"

#endif