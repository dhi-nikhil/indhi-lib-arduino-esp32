#ifndef UTILS_H_
#define UTILS_H_

#include "stdint.h"
#include "string"
#include "string.h"
#include "esp_err.h"
#include "vector"

static const char Base64AlphabetMap [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// namespace Utils
// {
class Utils
{
public:
    // static int Base64DecodedLength(size_t length);
    static int Base64DecodedLength(const std::string & data);
    static unsigned char Base64Lookup(unsigned char ch);
    static int Base64EncodedLength(size_t length);
    static int Base64EncodedLength(const std::string & data);
    static void A3toA4(unsigned char * a4, unsigned char * a3);
    static void A4toA3(unsigned char * a3, unsigned char * a4);
    static bool Base64Decode(std::string * destination, const std::string & source);
    static bool Base64Encode(std::string * destination, const std::string & source);
    static void dumpinfo();
    static bool EndsWith(std::string data, char c);
    static const char * ErrorToString(esp_err_t esp_err);
    // static const char* WiFiErrorToString(esp_err_t esp_err);
    // static void HexDump(const uint8_t *data, uint32_t length);
    static std::string IpToString(uint8_t * ip);
    static std::vector<std::string> split(std::string data, char delimiter);
    // static std::string ToLower(std::string &data);
    static std::string Trim(const std::string & data);
    static esp_err_t StringToHex(std::string input, unsigned char * output);
    static esp_err_t HexToString(unsigned char * input, int length, std::string * output);
    static esp_err_t URLDecode(const char * input, char * output);
    static bool IsHex(int hex);
};

// } // namespace Idf

#endif
