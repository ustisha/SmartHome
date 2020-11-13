#ifndef DISPLAYNET_H
#define DISPLAYNET_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <DisplaySSD1289.h>
#include <ClockNet.h>
#include <Net.h>

class DisplaySSD1289Net : public NetComponent {

public:
    DisplaySSD1289Net(SmartNet *n, uint8_t sp, uint8_t max, DisplaySSD1289 *dw) : NetComponent(n, sp, max) {
        displayView = dw;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override {

    }

    void receiveCommandData(Packet *p) override;

protected:
    DisplaySSD1289 *displayView;
};


#endif //DISPLAYNET_H
