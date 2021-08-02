#ifndef HELPER_H_
#define HELPER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define ERROR_CHECK(x)                                            \
    do                                                            \
    {                                                             \
        esp_err_t __err_rc = (x);                                 \
        if (__err_rc != ESP_OK)                                   \
        {                                                         \
            _esp_error_check_failed(__err_rc, __FILE__, __LINE__, \
                                    __ASSERT_FUNC, #x);           \
        }                                                         \
    } while (0)

#define ERROR_RETURN(x, f) \
    {                      \
        if (x != ESP_OK)   \
        {                  \
            return f;      \
        }                  \
    }

// #define ERROR_CHECK(x)
//     do
//     {
//         esp_err_t __err_rc = (x);
//         if (__err_rc != ESP_OK)
//         {
//             _esp_error_check_failed(__err_rc, __FILE__, __LINE__,
//                                     __ASSERT_FUNC, #x);
//         }
//     } while (0)

// #define ERROR_RETURN(x, f)
//     {
//         if (x != ESP_OK)
//         {
//             ESP_ERROR_CHECK_WITHOUT_ABORT(x);
//             return f;
//         }
//     }

#endif
