#ifndef SSL_UTILS_H_
#define SSL_UTILS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string"
#include "esp_event_base.h"
#include "esp_event.h"


// MAKE THIS EITHER ABLE TO STORE CERT WITH KEY OR MAKE THIS INSTANSABLE AND PASS THE INSTANCE TO ALL OTHER
// namespace Idf
// {
//     namespace SSL
//     {
/**
 * @brief SSL utility class
 *
 */
class SSLUtils
{
private:
    char * mCertificate; /* certificate */
    char * mKey; /* key */

public:
    /**
     * @brief Construct a new SSLUtils object
     *
     */
    SSLUtils();
    /**
     * @brief Destroy the SSLUtils object
     *
     */
    ~SSLUtils();
    /**
     * @brief Set the Certificate object
     *
     * @param certificate certificate to store
     */
    void SetCertificate(std::string certificate);
    /**
     * @brief Get the Certificate object
     *
     * @return char* get the certificate
     */
    char * GetCertificate();
    /**
     * @brief Set the Key object
     *
     * @param key Set the key
     */
    void SetKey(std::string key);
    /**
     * @brief Get the Key object
     *
     * @return char* Get the key
     */
    char * GetKey();
};
//     }
// }

#endif
