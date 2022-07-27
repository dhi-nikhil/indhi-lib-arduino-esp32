#ifndef PAYLOAD_ENCRYPTION_H_
#define PAYLOAD_ENCRYPTION_H_

#include "freertos/FreeRTOS.h"
#include "mbedtls/aes.h"
#include "mbedtls/base64.h"
#include "string"
#include "string.h"
#include "System.h"
#include "Utils.h"

/**
 * @brief Payload encryption class
 *
 */
class PayloadEncryption
{
private:
    static mbedtls_aes_context mContext; /* aes context */
    static unsigned char mEncryptionKey[32]; /* Encryption key */
public:
    /**
     * @brief Construct a new Payload Encryption object
     *
     */
    PayloadEncryption();
    /**
     * @brief Destroy the Payload Encryption object
     *
     */
    ~PayloadEncryption();
    /**
     * @brief Static method to Initialize the encryption
     *
     */
    static void Init();
    /**
     * @brief Static method to Deinitialize the encryption
     *
     */
    static void Deinit();
    /**
     * @brief Set the Encryption Key
     *
     * @param encryptionKey Encryption key to set
     * @return esp_err_t return if success or failed
     */
    static esp_err_t SetEncryptionKey(std::string encryptionKey);
    /**
     * @brief Set the Encryption Key
     *
     * @param encryptionKey Encryption key to set
     * @return esp_err_t return if success or failed
     */
    static esp_err_t SetEncryptionKey(unsigned char * encryptionKey);
    /**
     * @brief Encrypt the data using CBC
     *
     * @param output store the encrypted data
     * @param iv IV of the encrypted data
     * @param input input data string
     * @return esp_err_t return if success or failed
     */
    static esp_err_t EncryptCBC(std::string * output, std::string iv, std::string input);
    /**
     * @brief Encrypt the data using CBC
     *
     * @param output store the encrypted data
     * @param iv IV of the encrypted data
     * @param input input data string
     * @return esp_err_t return if success or failed
     */
    static esp_err_t EncryptCBC(std::string * output, std::string * iv, std::string input);
    /**
     * @brief Encrypt the data using CBC
     *
     * @param output store the encrypted data
     * @param iv IV of the encrypted data
     * @param input input data string
     * @return esp_err_t return if success or failed
     */
    static esp_err_t DecryptCBC(std::string * output, std::string iv, std::string input);
};

#endif
