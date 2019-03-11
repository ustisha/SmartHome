#ifndef NETCOMPONENT_H
#define NETCOMPONENT_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <SmartNet.h>

class NetComponent {

    const static byte MAX = 3;

    struct R {
        R() : network(NULL), receiver(0), rport(0), cmd(0), timeout(0), last(0) {
        }

        RadioInterface *network;
        uint8_t receiver;
        uint16_t rport;
        uint8_t cmd;
        uint32_t timeout;
        uint32_t last;
    };

public:
    NetComponent(uint16_t sp, SmartNet *n);

    virtual ~NetComponent() {};

    void addReceiver(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t c, float t);

    virtual void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) = 0;

    void tick(uint16_t sleep = 0);

protected:
    SmartNet *net;
    uint16_t sport;
    uint32_t sleepTime;
    R rcvr[NetComponent::MAX];

    uint8_t getIndex();
};

#endif //NETCOMPONENT_H
