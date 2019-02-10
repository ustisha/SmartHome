#ifndef NETCOMPONENT_H
#define NETCOMPONENT_H

//class SmartNet;

#define SERIAL_DEBUG

#include <Arduino.h>
#include <SmartNet.h>

class NetComponent {

    const static byte MAX = 3;

    struct R {
        R() : network(NULL), receiver(0), rport(0), timeout(0), cmd(0), last(0) {
        }

        Radio *network;
        uint8_t receiver;
        uint16_t rport;
        uint16_t timeout;
        uint8_t cmd;
        unsigned long last;
    };

public:
    NetComponent(uint16_t sp, SmartNet *n);

    virtual ~NetComponent() {};

    void addReceiver(Radio *n, uint8_t r, uint16_t rp, uint8_t c, uint16_t t);

    virtual void sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) = 0;

    void tick();

protected:
    SmartNet *net;
    uint16_t sport;
    R rcvr[NetComponent::MAX];

    byte getIndex();
};

#endif //NETCOMPONENT_H
