#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "PayloadEncryption.h"
#include "esp_log.h"
#include "string"
#include "string.h"
#include "esp_err.h"

static const char * TAG = "PayloadEncryption";

mbedtls_aes_context PayloadEncryption::mContext = { 0 };

unsigned char PayloadEncryption::mEncryptionKey[32] = { 0 };

PayloadEncryption::PayloadEncryption()
{
    Init();
}

void PayloadEncryption::Init()
{
    ::mbedtls_aes_init(&mContext);
    ESP_LOGI(TAG, "Initialized");
}

void PayloadEncryption::Deinit()
{
    ::mbedtls_aes_free(&mContext);
    ESP_LOGI(TAG, "Deinitialized");
}

esp_err_t PayloadEncryption::SetEncryptionKey(std::string encryptionKey)
{
    Utils::StringToHex(encryptionKey, &mEncryptionKey[0]);
    int error = mbedtls_aes_setkey_enc(&mContext, &mEncryptionKey[0], 256);
    if (error != 0)
        return ESP_FAIL;
    return ESP_OK;
}

esp_err_t PayloadEncryption::SetEncryptionKey(unsigned char * encryptionKey)
{
    memcpy(&mEncryptionKey[0], encryptionKey, 32);
    int error = mbedtls_aes_setkey_enc(&mContext, &mEncryptionKey[0], 256);
    if (error != 0)
        return ESP_FAIL;
    return ESP_OK;
}

esp_err_t PayloadEncryption::EncryptCBC(std::string * output, std::string iv, std::string input)
{
    int error = 0;
    size_t input_length = input.length();
    size_t input_data_length_after_encryption = ( ( input_length / 16 ) + 1 ) * 16;
    size_t input_data_padding = input_data_length_after_encryption - input_length;
    unsigned char padding_character = input_data_padding;
    size_t input_data_length_after_encoding = 4 * ( input_data_length_after_encryption / 3 + ( input_data_length_after_encryption % 3 != 0 ) ) + 1;
    size_t encoded_data_length = 0;

    unsigned char input_data[input_data_length_after_encryption] = { 0 };
    unsigned char encrypted_data[input_data_length_after_encryption] = { 0 };
    unsigned char encoded_data[input_data_length_after_encoding] = { 0 };

    memcpy(&input_data[0], input.c_str(), input_length);
    memset(&input_data[input_length], padding_character, input_data_padding);

    unsigned char i_v[16] = { 0 };
    Utils::StringToHex(iv, &i_v[0]);
    ESP_LOG_BUFFER_HEX_LEVEL("IV", &i_v[0], 16, ESP_LOG_INFO);

    error = mbedtls_aes_crypt_cbc(&mContext, MBEDTLS_AES_ENCRYPT, input_data_length_after_encryption, &i_v[0], &input_data[0], &encrypted_data[0]);
    if (error != 0)
    {
        ESP_LOGE(TAG, "Error %d (%2x) while encrypting data", error, error);
        return ESP_FAIL;
    }

    error = mbedtls_base64_encode(encoded_data, input_data_length_after_encoding, &encoded_data_length, &encrypted_data[0], input_data_length_after_encryption);
    if (error != 0)
    {
        ESP_LOGI(TAG, "encoded data length : %d", encoded_data_length);
        ESP_LOGE(TAG, "Error %d (%2x) while encoding data", error, error);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "input data length : %d", input_length);
    ESP_LOGI(TAG, "encoded data length : %d", encoded_data_length);
    ESP_LOGI(TAG, "input data length after encryption : %d", input_data_length_after_encryption);
    ESP_LOGI(TAG, "padding character : %d", padding_character);
    ESP_LOG_BUFFER_HEX_LEVEL("Input Data", input_data, input_data_length_after_encryption, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEX_LEVEL("Encryption Key", mEncryptionKey, 32, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEX_LEVEL("Encrypted Data", encrypted_data, input_data_length_after_encryption, ESP_LOG_INFO);

    *output = std::string((char *) encoded_data, encoded_data_length);

    return ESP_OK;
}

esp_err_t PayloadEncryption::EncryptCBC(std::string * output, std::string * iv, std::string input)
{
    int error = 0;
    size_t input_length = input.length();
    size_t input_data_length_after_encryption = ( ( input_length / 16 ) + 1 ) * 16;
    size_t input_data_padding = input_data_length_after_encryption - input_length;
    unsigned char padding_character = input_data_padding;
    size_t input_data_length_after_encoding = 4 * ( input_data_length_after_encryption / 3 + ( input_data_length_after_encryption % 3 != 0 ) ) + 1;
    size_t encoded_data_length = 0;

    unsigned char input_data[input_data_length_after_encryption] = { 0 };
    unsigned char encrypted_data[input_data_length_after_encryption] = { 0 };
    unsigned char encoded_data[input_data_length_after_encoding] = { 0 };

    memcpy(&input_data[0], input.c_str(), input_length);
    memset(&input_data[input_length], padding_character, input_data_padding);

    unsigned char i_v[16] = { 0 };
    esp_fill_random(i_v, 16);
    Utils::HexToString(&i_v[0], 16, iv);
    ESP_LOG_BUFFER_HEX_LEVEL("IV", &i_v[0], 16, ESP_LOG_INFO);

    error = mbedtls_aes_crypt_cbc(&mContext, MBEDTLS_AES_ENCRYPT, input_data_length_after_encryption, &i_v[0], &input_data[0], &encrypted_data[0]);
    if (error != 0)
    {
        ESP_LOGE(TAG, "Error %d (%2x) while encrypting data", error, error);
        return ESP_FAIL;
    }

    error = mbedtls_base64_encode(encoded_data, input_data_length_after_encoding, &encoded_data_length, &encrypted_data[0], input_data_length_after_encryption);
    if (error != 0)
    {
        ESP_LOGI(TAG, "encoded data length : %d", encoded_data_length);
        ESP_LOGE(TAG, "Error %d (%2x) while encoding data", error, error);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "input data length : %d", input_length);
    ESP_LOGI(TAG, "encoded data length : %d", encoded_data_length);
    ESP_LOGI(TAG, "input data length after encryption : %d", input_data_length_after_encryption);
    ESP_LOGI(TAG, "padding character : %d", padding_character);
    ESP_LOG_BUFFER_HEX_LEVEL("Input Data", input_data, input_data_length_after_encryption, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEX_LEVEL("Encryption Key", mEncryptionKey, 32, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEX_LEVEL("Encrypted Data", encrypted_data, input_data_length_after_encryption, ESP_LOG_INFO);

    *output = std::string((char *) encoded_data, encoded_data_length);

    return ESP_OK;
}

esp_err_t PayloadEncryption::DecryptCBC(std::string * output, std::string iv, std::string input)
{
    int error = 0;
    size_t input_length = input.length();
    size_t input_data_length_after_decoding = 512;
    // size_t input_data_length_after_decoding = 4 * ( input_data_length_after_encryption / 3 + ( input_data_length_after_encryption % 3 != 0 ) ) + 1;
    size_t decoded_data_length = 0;
    size_t output_data_padding_length = 0;

    unsigned char input_data[input_length] = { 0 };
    unsigned char decrypted_data[input_data_length_after_decoding] = { 0 };
    unsigned char decoded_data[input_data_length_after_decoding] = { 0 };

    memcpy(&input_data[0], input.c_str(), input_length);

    unsigned char i_v[16] = { 0 };
    Utils::StringToHex(iv, &i_v[0]);

    error = mbedtls_base64_decode(&decoded_data[0], input_data_length_after_decoding, &decoded_data_length, &input_data[0], input_length);
    if (error != 0)
    {
        ESP_LOGI(TAG, "decoded data length : %d", decoded_data_length);
        ESP_LOGE(TAG, "Error %d (%2x) while encoding data", error, error);
        return ESP_FAIL;
    }

    error = mbedtls_aes_crypt_cbc(&mContext, MBEDTLS_AES_DECRYPT, decoded_data_length, i_v, &decoded_data[0], &decrypted_data[0]);
    if (error != 0)
    {
        ESP_LOGE(TAG, "Error %d (%2x) while decrypting data", error, error);
        return ESP_FAIL;
    }

    output_data_padding_length = decrypted_data[decoded_data_length - 1];
    memset(&decrypted_data[decoded_data_length - output_data_padding_length], 0, output_data_padding_length);

    ESP_LOGI(TAG, "input data length : %d", input_length);
    ESP_LOGI(TAG, "decoded data length : %d", decoded_data_length);
    ESP_LOG_BUFFER_HEX_LEVEL("Decrypted Data", decrypted_data, decoded_data_length, ESP_LOG_INFO);

    *output = std::string((char *) decrypted_data, decoded_data_length);

    return ESP_OK;
}

PayloadEncryption::~PayloadEncryption()
{
    Deinit();
    ESP_LOGI(TAG, "Destructed");
}
