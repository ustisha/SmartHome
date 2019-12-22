#ifndef NETCOMPONENT_H
#define NETCOMPONENT_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <SmartNet.h>
#include <RadioInterface.h>

class SmartNet;

class NetComponent {

    struct R {
        R() {
            enabled = false;
            receiver = 0;
            rport = 0;
            cmd = 0;
            timeout = 0;
            last = millis();
        }

        RadioInterface *network;
        bool enabled;
        uint8_t receiver;
        uint8_t rport;
        uint8_t cmd;
        uint32_t timeout;
        uint32_t last;
    };

public:
    NetComponent(SmartNet *n, uint8_t sp, uint8_t max);

    void addReceiver(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t c, float t);

    virtual void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) = 0;

    void receiveHandle(uint16_t rp, uint8_t cmd, long data);

    void tick(uint16_t sleep = 0);

protected:
    SmartNet *net;
    uint8_t sport;
    uint8_t i;
    uint8_t maxCmp;
    uint32_t sleepTime;
    R *rcvr;

    virtual void receiveCommandData(uint8_t cmd, long data) = 0;
};

#endif //NETCOMPONENT_H
