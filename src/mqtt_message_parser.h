#ifndef MQTT_MESSAGE_PARSER_H_
#define MQTT_MESSAGE_PARSER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "error_code_manager.h"
#include "helper.h"
#include "log_handler.h"
#include "cJSON.h"
#include "string.h"

typedef int activation_status_t;

// typedef enum
// {
//     VALUE_TYPE_NONE = 0,
//     VALUE_TYPE_INT,
//     VALUE_TYPE_STRING,
//     VALUE_TYPE_MAX,
// } value_type_t;

// typedef int int_value;

// typedef struct
// {
//     int len;
//     char *value;
// } char_value_t;

// typedef union
// {
//     int_value int_value;
//     char_value_t char_value;
// } data_t;

typedef struct
{
    int feature_index;
    int value;
} parser_t;

ErrorCode_t parse_value_from_string(char *data, parser_t *parsed_data);

ErrorCode_t mqtt_data_activation_status_parser(char *data, activation_status_t *activation_status);

#endif