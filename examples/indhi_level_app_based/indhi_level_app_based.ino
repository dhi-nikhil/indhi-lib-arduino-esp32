#include "indhilib.h"
#include "messaging.h"

void myTask(void *arg);

#define DEVICE_NAME "Light level"
#define FEATURE_1_ID "11111"
#define FEATURE_1_NAME "level light"

// copy this and paste above device_onboard function
void levelDevice(void *arg)
{
    while (true)
    {
        indhilib_send_data((char *)FEATURE_1_ID, esp_random() % 100);
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
    vTaskDelete(NULL);
}

ErrorCode_t device_onboard(void)
{
    ErrorCode_t ErrorCode = ESP_OK;
    indhilib_init();
    indhilib_set_device_name((char *)DEVICE_NAME);
    indhilib_set_onboarding_method(ONBOARDING_METHOD_APP_BASED);
    // Add features here
    ERROR_RETURN(indhilib_add_feature((char *)FEATURE_1_ID, (char *)FEATURE_1_NAME, FEATURE_CAP_PUBLISH, 1, NULL), ESP_FAIL);
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
        xTaskCreate((TaskFunction_t)levelDevice, "levelDevice", 4096, NULL, 5, NULL);
        messaging_init();
        messaging_start();
    }
    else
    {
        Serial.println("onboarding failed with error code : ");
        Serial.println(ErrorCode);
        Serial.flush();
        vTaskDelay(pdMS_TO_TICKS(5000));
        esp_restart();
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