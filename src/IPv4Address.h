#ifndef IP_ADDRESS_H_
#define IP_ADDRESS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_http_client.h"
#include "string"

class IPv4Address
{
private:
    union
    {
        uint8_t bytes[4];
        uint32_t dword;
    }_address;

    uint8_t * raw_address()
    {
        return _address.bytes;
    }

public:
    IPv4Address();
    IPv4Address(uint8_t firstOctate, uint8_t secondOctate, uint8_t thirdOctate, uint8_t fourthOctate);
    IPv4Address(uint32_t address);
    IPv4Address(const uint8_t * address);
    virtual ~IPv4Address() {}

    bool FromString(const char * address);
    bool FromString(std::string & address)
    {
        return FromString(address.c_str());
    }

    operator uint32_t() const
    {
        return _address.dword;
    }

    bool operator==(const IPv4Address & address) const
    {
        return _address.dword == address._address.dword;
    }

    bool operator==(const uint8_t * address) const;

    uint8_t  operator[](int index) const
    {
        return _address.bytes[index];
    }

    uint8_t & operator[](int index)
    {
        return _address.bytes[index];
    }

    IPv4Address & operator=(const uint8_t * address);
    IPv4Address & operator=(uint32_t address);

    // virtual size_t PrintTo()
    std::string ToString() const;
};

// const IPv4Address INADDR_NONE(0, 0, 0, 0);

#endif
