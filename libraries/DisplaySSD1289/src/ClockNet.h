#ifndef CLOCKNET_H
#define CLOCKNET_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <DisplaySSD1289.h>
#include <Clock.h>
#include <Net.h>

class ClockNet : public NetComponent {

public:
    ClockNet(SmartNet *n, uint8_t sp, uint8_t max, Clock *clock) : NetComponent(n, sp, max), clock(clock) {
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override {

    }

    void receiveCommandData(Packet *p) override;

protected:
    Clock *clock;

};


#endif //CLOCKNET_H
