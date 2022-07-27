#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "IPv4Address.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "string.h"
#include "string"

static const char * TAG = "IPv4Address";

// namespace Idf
// {
//     namespace HTTPClient
//     {
IPv4Address::IPv4Address()
{
    _address.dword = 0;
}

IPv4Address::IPv4Address(uint8_t firstOctate, uint8_t secondOctate, uint8_t thirdOctate, uint8_t fourthOctate)
{
    _address.bytes[0] = firstOctate;
    _address.bytes[1] = secondOctate;
    _address.bytes[2] = thirdOctate;
    _address.bytes[3] = fourthOctate;
}

IPv4Address::IPv4Address(uint32_t address)
{
    _address.dword = address;
}

IPv4Address::IPv4Address(const uint8_t * address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
}

bool FromString(const char * address);

bool FromString(std::string & address)
{
    return FromString(address.c_str());
}

IPv4Address & IPv4Address::operator = (const uint8_t * address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
    return *this;
}

IPv4Address & IPv4Address::operator = (uint32_t address)
{
    _address.dword = address;
    return *this;
}

bool IPv4Address::operator==(const uint8_t * address) const
{
    return memcmp(address, _address.bytes, sizeof(_address.bytes)) == 0;
}

std::string IPv4Address::ToString() const
{
    char szRet[16];
    sprintf(szRet, "%u.%u.%u.%u", _address.bytes[0], _address.bytes[1], _address.bytes[2], _address.bytes[3]);
    return std::string(szRet);
}

bool IPv4Address::FromString(const char * address)
{
    uint16_t acc = 0;
    uint8_t dots = 0;

    while (*address)
    {
        char c = *address++;
        if (c >= '0' && c <= '9')
        {
            acc = acc * 10 + ( c - '0' );
            if (acc > 255)
            {
                return false;
            }
        }
        else if (c == '.')
        {
            if (dots == 3)
            {
                return false;
            }
            _address.bytes[dots++] = acc;
            acc = 0;
        }
        else
        {
            return false;
        }
    }

    if (dots != 3)
    {
        return false;
    }
    _address.bytes[3] = acc;
    return true;
}

// } // HTTPClient
// } // Idf

