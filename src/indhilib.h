#ifndef INDHILIB_H_
#define INDHILIB_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "log_handler.h"
#include "error_code_manager.h"
#include "nvs_manager.h"
#include "network_manager.h"
#include "event_manager.h"
#include "wifi_manager.h"
#include "wifi_ap_manager.h"
#include "wifi_sta_manager.h"
#include "provision.h"
#include "api_client_validation.h"
#include "api_client_device_init.h"
#include "ping_ip4.h"

using namespace std;

#define CONFIG_DEVICE_NAME "esp32-arduino"

typedef double feature_status_t;
/**
 * @brief user to define a callback where data will get received
 * 
 */
typedef feature_status_t (*feature_callback_t)(char *feature_id, int data);

/* 9 (user id) + 25 (device_name + unique id) + 25 (feature_name + unique id) + type (6) + 3 + 1 */

typedef enum
{
    FEATURE_NO_CAP = 0,
    FEATURE_CAP_PUBLISH,
    FEATURE_CAP_PUBLISH_SUBSCRIBE,
    FEATURE_CAP_MAX,
} feature_type_t;

typedef enum
{
    ONBOARDING_METHOD_NONE = 0,
    ONBOARDING_METHOD_APP_BASED,
    ONBOARDING_METHOD_TOKEN_BASED,
    ONBOARDING_METHOD_MAX,
} onboarding_method_t;

typedef struct
{
    uint8_t on_level_value;
    uint8_t off_level_value;
} onoff_button_widget_t;

typedef struct
{
    uint8_t min_level_value;
    uint8_t max_level_value;
    uint8_t steps;
} level_bar_widget_t;

typedef struct
{
    uint8_t min_level_value;
    uint8_t max_level_value;
    uint8_t steps;
} color_pallet_widget_t;

typedef union
{
    onoff_button_widget_t onoff_button_widget;
    level_bar_widget_t level_bar_widget;
    color_pallet_widget_t color_pallet_widget;
} widget_data_t;

typedef enum
{
    WIDGET_CODE_NONE = 0,
    WIDGET_CODE_TEXT_FIELD,
    WIDGET_CODE_ONOFF_BUTTON,
    WIDGET_CODE_LEVEL_BAR,
    WIDGET_CODE_COLOR_PALLET,
    WIDGET_CODE_MAX
} widget_code_t;

typedef struct
{
    char name[17];
    int feature_type;
    int version_code;
    char feature_id[6];
    feature_callback_t feature_callback;
    char feature_pub_topic[100];
    char feature_sub_topic[100];
    widget_code_t widget_code;
    widget_data_t widget_data;
} feature_config_t;

// class indhilib
// {
// private:
//     string token;
//     int currentValue;
//     feature_config_t *features;

// public:
//     indhilib();
//     ~indhilib();
//     int indhilib_add_feature(char *name, int type);
//     char *indhilib_get_all_features();
//     char *indhilib_get_feature(int id);
//     ErrorCode_t indhilib_provisioning();
// };

char *indhilib_get_feature_pubtopic_from_index(int feature_index);
char *indhilib_get_feature_subtopic_from_index(int feature_index);
int indhilib_get_index_from_feature_subtopic(char *feature_subtopic);
int indhilib_get_index_from_feature_pubtopic(char *feature_pubtopic);
ErrorCode_t indhilib_pub_topic_set(char *feature_id, char *topic);
ErrorCode_t indhilib_sub_topic_set(char *feature_id, char *topic);
/**
 * @brief to send feature specific data
 * 
 * @param feature_id specify the feature id used in indhilib_add_feature
 * @param data specify data to send
 * @param qos specify the qos
 * @return ErrorCode_t success or failed
 */
ErrorCode_t indhilib_send_data(char *feature_id, double data, uint64_t timestamp);
/**
 * @brief to send feature specific data
 * 
 * @param feature_id specify the feature id used in indhilib_add_feature
 * @param data specify data to send
 * @param qos specify the qos
 * @return ErrorCode_t success or failed
 */
ErrorCode_t indhilib_send_data(char *feature_id, int data, uint64_t timestamp);
ErrorCode_t indhilib_send_data(char *feature_id, uint64_t data, uint64_t timestamp);
ErrorCode_t indhilib_send_data(char *feature_id, char *data, uint64_t timestamp);
ErrorCode_t indhilib_send_data(char *feature_id, double data);
ErrorCode_t indhilib_send_data(char *feature_id, int data);
ErrorCode_t indhilib_send_data(char *feature_id, uint64_t data);
ErrorCode_t indhilib_send_data(char *feature_id, char *data);
ErrorCode_t indhilib_device_init_create_request(char **req, int length);
char *indhilib_get_device_name(void);
ErrorCode_t indhilib_set_device_name(char *device_name);
ErrorCode_t indhilib_set_device_id(char *device_id);
ErrorCode_t indhilib_print_all_features();
feature_config_t *indhilib_get_feature(int index);
int indhilib_get_number_of_features(void);
/**
 * @brief add the features 
 * 
 * @param feature_id unique feature id
 * @param name unique feature name
 * @param type feature type
 * @param version_code version code for future use case
 * @param fc callback to the function where the data will get received if data arrives for that specific feature
 * @return ErrorCode_t 
 */
ErrorCode_t indhilib_add_onoff_button(char *feature_id, char *feature_name, uint8_t on_level, uint8_t off_level, int version_code, feature_callback_t fc);
ErrorCode_t indhilib_add_level_bar(char *feature_id, char *feature_name, uint8_t min_level, uint8_t max_level, uint8_t steps, int version_code, feature_callback_t fc);
ErrorCode_t indhilib_add_color_pallet(char *feature_id, char *feature_name, int version_code, feature_callback_t fc);
ErrorCode_t indhilib_add_feature(char *feature_id, char *feature_name, int type, widget_code_t widget_code, widget_data_t widget_data, int version_code, feature_callback_t fc);
ErrorCode_t indhilib_add_feature(char *feature_id, char *name, int type, int version_code, feature_callback_t fc);
ErrorCode_t indhilib_deinit(void);
ErrorCode_t check_configuration(void);
ErrorCode_t check_configuration_code(void);
ErrorCode_t indhilib_init(void);
ErrorCode_t indhilib_send_data_to_feature_callback(int index, int value);
ErrorCode_t indhilib_set_onboarding_method(onboarding_method_t onboardingmethod);
onboarding_method_t indhilib_get_onboarding_method(void);
ErrorCode_t indhilib_set_onboarding_token(char *token);
char *indhilib_get_onboarding_token(void);
#endif