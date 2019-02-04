#ifndef SMARTHOME_NETCOMPONENT_H
#define SMARTHOME_NETCOMPONENT_H

//class SmartNet;

#include <Arduino.h>
#include <SmartNet.h>

class NetComponent {

    const static byte MAX = 2;

    struct R {
        uint16_t receiver = NULL;
        uint16_t rport = NULL;
        uint16_t timeout = NULL;
        uint8_t cmd = NULL;
        unsigned long last = 0;
    };

public:
    NetComponent(uint16_t sp, SmartNet *n);

    void addReceiver(uint16_t r, uint16_t rp, uint8_t c, uint16_t t);

    virtual void sendCommandData(uint16_t r, uint16_t rp, uint8_t cmd) = 0;

    void tick();

protected:
    SmartNet *net;
    uint16_t sport;
    R rcvr[MAX];

    int getIndex();
};

#endif //SMARTHOME_NETCOMPONENT_H
