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
class SSLUtils
{
private:
    char * mCertificate;
    char * mKey;

public:
    SSLUtils();
    ~SSLUtils();
    void SetCertificate(std::string certificate);
    char * GetCertificate();
    void SetKey(std::string key);
    char * GetKey();
};
//     }
// }

#endif
