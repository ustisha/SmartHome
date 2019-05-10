#ifndef SMARTNET_H
#define SMARTNET_H

class NetComponent;

#include <Arduino.h>
#include <DebugLog.h>
#include <Net.h>
#include <NetComponent.h>

class SmartNet {
    const static uint8_t MAX = 10;
    struct NetComponents {
        bool enabled;
        NetComponent *netComponent;
    };

public:
    SmartNet(uint16_t s);

    void addNetComponent(NetComponent *nc);

    void commandReceived(Packet packet);

    uint8_t getSender();

protected:

    int getIndex();

    NetComponents components[MAX];
    uint8_t sender;
};

#endif //SMARTNET_H
