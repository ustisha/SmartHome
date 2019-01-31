#ifndef SMARTHOME_NETCOMPONENT_H
#define SMARTHOME_NETCOMPONENT_H

#include <Arduino.h>
#include <SmartNet.h>

class NetComponent {
public:
    NetComponent(uint16_t p);

    void addNet(SmartNet *n);

protected:
    SmartNet *net;
    uint16_t port;
};

#endif //SMARTHOME_NETCOMPONENT_H
