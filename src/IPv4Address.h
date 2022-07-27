#ifndef IP_ADDRESS_H_
#define IP_ADDRESS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_http_client.h"
#include "string"

/**
 * @brief IPv4Address class
 *
 */
class IPv4Address
{
private:
    /**
     * @brief union to store the address
     *
     */
    union
    {
        uint8_t bytes[4];
        uint32_t dword;
    }_address;

    /**
     * @brief Get the raw address
     *
     * @return uint8_t*
     */
    uint8_t * raw_address()
    {
        return _address.bytes;
    }

public:
    /**
     * @brief Construct a new IPv4Address object
     *
     */
    IPv4Address();
    /**
     * @brief Construct a new IPv4Address object
     *
     * @param firstOctate of the IP address
     * @param secondOctate  of the IP address
     * @param thirdOctate  of the IP address
     * @param fourthOctate  of the IP address
     */
    IPv4Address(uint8_t firstOctate, uint8_t secondOctate, uint8_t thirdOctate, uint8_t fourthOctate);
    /**
     * @brief Construct a new IPv4Address object
     *
     * @param address complete ip address
     */
    IPv4Address(uint32_t address);
    /**
     * @brief Construct a new IPv4Address object
     *
     * @param address Complete ip address
     */
    IPv4Address(const uint8_t * address);
    /**
     * @brief Destroy the IPv4Address object
     *
     */
    virtual ~IPv4Address() {}

    /**
     * @brief Fetch from character pointer
     *
     * @param address
     * @return bool success or failed
     */
    bool FromString(const char * address);
    /**
     * @brief Fetch IP from string
     *
     * @param address
     * @return bool success or failed
     */
    bool FromString(std::string & address)
    {
        return FromString(address.c_str());
    }

    operator uint32_t() const
    {
        return _address.dword;
    }

    /**
     * @brief Operator overloading ==
     *
     * @param address
     * @return true
     * @return false
     */
    bool operator==(const IPv4Address & address) const
    {
        return _address.dword == address._address.dword;
    }

    /**
     * @brief Operator overloading ==
     *
     * @param address
     * @return true
     * @return false
     */
    bool operator==(const uint8_t * address) const;

    /**
     * @brief Operator overloading []
     *
     * @param address
     * @return true
     * @return false
     */
    uint8_t  operator[](int index) const
    {
        return _address.bytes[index];
    }

    /**
     * @brief Operator overloading []
     *
     * @param address
     * @return true
     * @return false
     */
    uint8_t & operator[](int index)
    {
        return _address.bytes[index];
    }

    /**
     * @brief Operator overloading =
     *
     * @param address IP address
     * @return IPv4Address& to assign
     */
    IPv4Address & operator=(const uint8_t * address);
    IPv4Address & operator=(uint32_t address);

    // virtual size_t PrintTo()
    /**
     * @brief Convert IP to string
     *
     * @return std::string
     */
    std::string ToString() const;
};

// const IPv4Address INADDR_NONE(0, 0, 0, 0);

#endif
