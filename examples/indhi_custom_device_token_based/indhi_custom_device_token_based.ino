#include "indhilib.h"
#include "messaging.h"

#define TOKEN ""
#define SSID ""
#define PSK ""
#define DEVICE_NAME "My device"
#define FEATURE_1_ID "11111"
#define FEATURE_1_NAME "on off light"
#define FEATURE_2_ID "22222"
#define FEATURE_2_NAME "Light sensor"
#define FEATURE_3_ID "33333"
#define FEATURE_3_NAME "Voltage sensor"
#define FEATURE_4_ID "44444"
#define FEATURE_4_NAME "Current sensor"
#define FEATURE_5_ID "55555"
#define FEATURE_5_NAME "Level"

void myTask(void *arg);

feature_status_t onoffLight_callback(char *feature_id, int data)
{
    gpio_set_level(GPIO_NUM_2, data);
    if (data == 1)
    {
        indhilib_send_data((char *)FEATURE_2_ID, 100);
        indhilib_send_data((char *)FEATURE_3_ID, 230);
        indhilib_send_data((char *)FEATURE_4_ID, 0.4);
        indhilib_send_data((char *)FEATURE_5_ID, 0);
    }
    else
    {
        indhilib_send_data((char *)FEATURE_2_ID, 0);
        indhilib_send_data((char *)FEATURE_3_ID, 240);
        indhilib_send_data((char *)FEATURE_4_ID, 0.0);
        indhilib_send_data((char *)FEATURE_5_ID, 0);
    }
    return data;
}

feature_status_t levelLight_callback(char *feature_id, int data)
{
    return data;
}

void onoffLight(void *arg)
{
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLDOWN_ONLY);
    gpio_set_level(GPIO_NUM_2, 0);
    indhilib_send_data((char *)FEATURE_1_ID, gpio_get_level(GPIO_NUM_2), 0);
    indhilib_send_data((char *)FEATURE_2_ID, 75);
    indhilib_send_data((char *)FEATURE_3_ID, 230);
    indhilib_send_data((char *)FEATURE_4_ID, 0.4);
    indhilib_send_data((char *)FEATURE_5_ID, 50);
    vTaskDelete(NULL);
}

ErrorCode_t device_onboard(void)
{
    ErrorCode_t ErrorCode = ESP_OK;
    indhilib_init();
    indhilib_set_device_name((char *)DEVICE_NAME);
    indhilib_set_onboarding_token((char *)TOKEN);
    indhilib_set_router_credential((char *)SSID, (char *)PSK);
    indhilib_set_onboarding_method(ONBOARDING_METHOD_TOKEN_BASED);
    // Add features here
    ERROR_RETURN(indhilib_add_feature((char *)FEATURE_1_ID, (char *)FEATURE_1_NAME, FEATURE_CAP_PUBLISH_SUBSCRIBE, 1, (feature_callback_t)onoffLight_callback), ESP_FAIL);
    ERROR_RETURN(indhilib_add_feature((char *)FEATURE_2_ID, (char *)FEATURE_2_NAME, FEATURE_CAP_PUBLISH, 1, NULL), ESP_FAIL);
    ERROR_RETURN(indhilib_add_feature((char *)FEATURE_3_ID, (char *)FEATURE_3_NAME, FEATURE_CAP_PUBLISH, 1, NULL), ESP_FAIL);
    ERROR_RETURN(indhilib_add_feature((char *)FEATURE_4_ID, (char *)FEATURE_4_NAME, FEATURE_CAP_PUBLISH, 1, NULL), ESP_FAIL);
    ERROR_RETURN(indhilib_add_feature((char *)FEATURE_5_ID, (char *)FEATURE_5_NAME, FEATURE_CAP_PUBLISH_SUBSCRIBE, 1, (feature_callback_t)levelLight_callback), ESP_FAIL);
    ErrorCode = indhilib_provisioning();
    return ErrorCode;
}

void myTask(void *arg)
{
    ErrorCode_t ErrorCode = device_onboard();
    if (ErrorCode == ESP_OK)
    {
        // Add tasks here
        Serial.println("onboarding success");
        xTaskCreate((TaskFunction_t)onoffLight, "onoffLight", 4096, NULL, 5, NULL);
        messaging_init();
        messaging_start();
    }
    else
    {
        Serial.println("onboarding failed with error code : ");
        Serial.println(ErrorCode);
        Serial.flush();
    }
    vTaskDelete(NULL);
}

void reset_button_task(void *arg)
{
    int count = 0;
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        int level = gpio_get_level(GPIO_NUM_0);
        if (level == 0)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        if (count >= 5)
        {
            non_volatile_storage_erase_data();
            esp_restart();
        }
    }
    vTaskDelete(NULL);
}

void setup()
{
    Serial.begin(115200);
    xTaskCreate(reset_button_task, "reset_button_task", 4096, NULL, 5, NULL);
    xTaskCreatePinnedToCore(myTask, "test", 8192 * 2, NULL, 5, NULL, 0);
}

void loop()
{
}
