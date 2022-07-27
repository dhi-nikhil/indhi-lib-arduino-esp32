#ifndef NETWORK_INTERFACE_H_
#define NETWORK_INTERFACE_H_

#include "esp_netif.h"
#include "esp_log.h"

/**
 * @brief Network Interface class
 *
 */
class NetworkInterface
{
private:

public:
    /**
     * @brief Construct a new Network Interface object
     *
     */
    NetworkInterface();
    /**
     * @brief Destroy the Network Interface object
     *
     */
    ~NetworkInterface();
    /**
     * @brief Init the network interface
     *
     * @return esp_err_t return if success or not
     */
    esp_err_t Init();
    /**
     * @brief Deinit the network interface
     *
     * @return esp_err_t Return if success or not
     */
    esp_err_t Deinit();
};


#endif
