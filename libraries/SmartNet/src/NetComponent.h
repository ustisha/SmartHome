#ifndef SMARTHOME_NETCOMPONENT_H
#define SMARTHOME_NETCOMPONENT_H

class SmartNet;

#include <Arduino.h>
#include <SmartNet.h>

class NetComponent {
public:
    NetComponent(uint16_t sp);

    void addNet(SmartNet *n, uint16_t r, uint16_t rp);

protected:
    SmartNet *net;
    uint16_t sport;
    uint16_t receiver;
    uint16_t rport;
};

#endif //SMARTHOME_NETCOMPONENT_H
