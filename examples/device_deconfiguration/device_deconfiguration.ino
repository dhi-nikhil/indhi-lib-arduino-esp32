#include "indhilib.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("ESP32 idf Developer lib source code\n");
    Serial.println("Device deconfiguration code\n");
    non_volatile_storage_init();
    non_volatile_storage_erase_data();
    Serial.println("Device is deconfigured now\n");
}

void loop()
{
}