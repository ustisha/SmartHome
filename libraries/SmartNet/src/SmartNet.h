//#define SERIAL_DEBUG

#ifndef SMARTNET_H
#define SMARTNET_H

class NetComponent;

#include <Arduino.h>
#include <DebugLog.h>
#include <Net.h>
#include <NetComponent.h>

class SmartNet {
    struct NetComponents {
        bool enabled = false;
        NetComponent *netComponent;
    };

public:
    uint8_t sender;

    SmartNet(uint16_t s, uint8_t max);

    void addNetComponent(NetComponent *nc);

    void commandReceived(Packet *p);

protected:
    NetComponents *components;
    uint8_t i;
    uint8_t maxCmp;
};

#endif //SMARTNET_H
