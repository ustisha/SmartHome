//#define SERIAL_DEBUG

#ifndef SMARTNET_H
#define SMARTNET_H

class NetComponent;

#include <Arduino.h>
#include <DebugLog.h>
#include <Net.h>
#include <RadioInterface.h>

class SmartNet {
    struct NetComponents {
        NetComponents() {
            netComponent = nullptr;
        }

        NetComponent *netComponent;
    };

public:
    uint8_t sender;

    SmartNet(uint16_t s, uint8_t max) : components(new NetComponents[max]{}),
                                        sender(s),
                                        maxCmp(max) {}

    void sendInfo(RadioInterface *n, long data, uint8_t cmd = CMD_INFO);

    void addNetComponent(NetComponent *nc);

    void commandReceived(Packet *p);

protected:
    NetComponents *components;
    uint8_t i = 0;
    uint8_t maxCmp;
};

#endif //SMARTNET_H
