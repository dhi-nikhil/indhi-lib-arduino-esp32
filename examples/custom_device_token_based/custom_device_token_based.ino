#include "indhilib.h"

#define TOKEN ""
#define SSID ""
#define PSK ""
#define DEVICE_NAME "Custom Device"
#define FEATURE_1_ID "11111"
#define FEATURE_1_NAME "Light"
#define FEATURE_2_ID "22222"
#define FEATURE_2_NAME "Voltage"
#define FEATURE_3_ID "33333"
#define FEATURE_3_NAME "Current"

feature_status_t onoffLight_callback(char *feature_id, int data)
{
    digitalWrite(2, data);
    if (data == 1)
    {
        indhilib_send_data(FEATURE_2_ID, 230);
        indhilib_send_data(FEATURE_3_ID, 0.4);
    }
    else
    {
        indhilib_send_data(FEATURE_2_ID, 240);
        indhilib_send_data(FEATURE_3_ID, 0);
    }
    return data;
}

ErrorCode_t device_onboard(void)
{
    indhilib_init();
    indhilib_set_device_name(DEVICE_NAME);
    indhilib_set_onboarding_method(ONBOARDING_METHOD_TOKEN_BASED);
    indhilib_set_onboarding_token(TOKEN);
    indhilib_set_router_credential(SSID, PSK);
    // Add custom features here
    indhilib_add_feature(FEATURE_1_ID, FEATURE_1_NAME, 1, onoffLight_callback);
    indhilib_add_feature(FEATURE_2_ID, FEATURE_2_NAME, 1);
    indhilib_add_feature(FEATURE_3_ID, FEATURE_3_NAME, 1);
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
    Serial.println("Custom device with token based onboarding example with indhi");
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