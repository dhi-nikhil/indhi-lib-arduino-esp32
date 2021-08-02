#include "indhilib.h"
#include "messaging.h"

void setup()
{
    non_volatile_storage_init();
    non_volatile_storage_erase_data();
    esp_restart();
}

void loop()
{
}