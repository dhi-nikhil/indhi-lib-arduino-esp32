#include "indhilib.h"

#define DEVICE_NAME "Light"
#define FEATURE_1_ID "11111"
#define FEATURE_1_NAME "Light"

feature_status_t onoffLight_callback(char *feature_id, int data)
{
    /* Controlling the onboard led with the data */
    digitalWrite(2, data);
    return data;
}

ErrorCode_t device_onboard(void)
{
    indhilib_init();
    indhilib_set_device_name(DEVICE_NAME);
    indhilib_set_onboarding_method(ONBOARDING_METHOD_APP_BASED);
    indhilib_enable_wifi_onboarding();

    // Add features here
    indhilib_add_onoff_button(FEATURE_1_ID, FEATURE_1_NAME, 1, 0, 1, onoffLight_callback);
    return indhilib_provisioning();
}

void reset_button_task(void *arg)
{
    int count = 0;
    pinMode(0, INPUT);
    while (true)
    {
        delay(1000);
        int level = digitalRead(0);
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
    Serial.println("ESP32 Arduino Developer lib source code");
    Serial.println("On off light using wifi onboarding example with indhi");
    xTaskCreate(reset_button_task, "reset_button_task", 4096, NULL, 5, NULL);

    /* Light initialized here */
    pinMode(2, OUTPUT);
    digitalWrite(2, 0);

    ErrorCode_t ErrorCode = device_onboard();
    if (ErrorCode == ESP_OK)
    {
        Serial.println("onboarding success");
    }
    else
    {
        Serial.print("onboarding failed with error code : ");
        Serial.println(ErrorCode);
        delay(10000);
        esp_restart();
    }
}

void loop()
{
}