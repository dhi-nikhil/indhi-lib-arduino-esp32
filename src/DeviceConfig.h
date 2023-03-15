#ifndef DEVICE_CONFIG_H_
#define DEVICE_CONFIG_H_

#include "string"

class DeviceConfig
{
private:

    std::string mVar;
    double mValue;

protected:

    friend class Provision;
    friend class Device;

public:
    DeviceConfig(std::string var, double value);

     ~DeviceConfig();
};

#endif
