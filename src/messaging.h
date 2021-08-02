#ifndef MESSAGING_H_
#define MESSAGING_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_message_parser.h"

ErrorCode_t messaging_init(void);
ErrorCode_t messaging_deinit(void);
ErrorCode_t messaging_start(void);
ErrorCode_t messaging_stop(void);
ErrorCode_t message_rx_add_to_queue(parser_t *data);
ErrorCode_t message_rx_fetch_from_queue(parser_t *data);
ErrorCode_t message_queue_clear(void);

#endif