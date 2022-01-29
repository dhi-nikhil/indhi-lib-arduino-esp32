#ifndef NETWORK_INTERFACE_H_
#define NETWORK_INTERFACE_H_

#include "esp_netif.h"
#include "esp_log.h"

class NetworkInterface
{
private:

public:
    NetworkInterface();
    ~NetworkInterface();
    esp_err_t Init();
    esp_err_t Deinit();
};


#endif
