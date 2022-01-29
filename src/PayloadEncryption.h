#ifndef PAYLOAD_ENCRYPTION_H_
#define PAYLOAD_ENCRYPTION_H_

#include "freertos/FreeRTOS.h"
#include "mbedtls/aes.h"
#include "mbedtls/base64.h"
#include "string"
#include "string.h"
#include "System.h"
#include "Utils.h"


class PayloadEncryption
{
private:
    static mbedtls_aes_context mContext;
    static unsigned char mEncryptionKey[32];
public:
    PayloadEncryption();
    ~PayloadEncryption();
    static void Init();
    static void Deinit();
    static esp_err_t SetEncryptionKey(std::string encryptionKey);
    static esp_err_t SetEncryptionKey(unsigned char * encryptionKey);
    static esp_err_t EncryptCBC(std::string * output, std::string iv, std::string input);
    static esp_err_t EncryptCBC(std::string * output, std::string * iv, std::string input);
    static esp_err_t DecryptCBC(std::string * output, std::string iv, std::string input);
};

#endif
