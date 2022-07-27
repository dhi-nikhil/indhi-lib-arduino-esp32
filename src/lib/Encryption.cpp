#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Encryption.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char * TAG = "Encryption";

// namespace Idf
// {
//     namespace Encryption
//     {
        // PayloadEncryption::PayloadEncryption()
        // {
        // }

        // PayloadEncryption::~PayloadEncryption()
        // {
        // }

//     } // namespace Encryption
// } // namespace Idf
