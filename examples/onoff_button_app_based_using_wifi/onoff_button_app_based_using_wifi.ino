#include "indhilib.h"

#define DEVICE_NAME "Light"
#define FEATURE_1_ID "11111"
#define FEATURE_1_NAME "Light"

feature_status_t onoffLight_callback(char *feature_id, int data)
{
    /* Controlling the onboard led with the data */
    gpio_set_level(GPIO_NUM_2, data);
    return data;
}

ErrorCode_t device_onboard(void)
{
    ErrorCode_t ErrorCode = ESP_OK;
    indhilib_init();
    indhilib_set_device_name(DEVICE_NAME);
    indhilib_set_onboarding_method(ONBOARDING_METHOD_APP_BASED);
    indhilib_enable_wifi_onboarding();

    // Add features here
    indhilib_add_onoff_button(FEATURE_1_ID, FEATURE_1_NAME, 1, 0, 1, onoffLight_callback);
    ErrorCode = indhilib_provisioning();
    return ErrorCode;
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

extern "C" void app_main(void)
{
    Serial.begin(115200);
    Serial.println("ESP32 idf Developer lib source code");
    Serial.println("On off light using wifi onboarding example with indhi");
    xTaskCreate(reset_button_task, "reset_button_task", 4096, NULL, 5, NULL);

    /* Light initialized here */
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLDOWN_ONLY);
    gpio_set_level(GPIO_NUM_2, 0);

    ErrorCode_t ErrorCode = device_onboard();
    if (ErrorCode == ESP_OK)
    {
        Serial.println("onboarding success");
    }
    else
    {
        Serial.println("onboarding failed with error code : " + ErrorCode);
        vTaskDelay(pdMS_TO_TICKS(10000));
        esp_restart();
    }
}

void loop()
{
}