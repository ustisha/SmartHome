#ifndef TEMPHUMPRESSURENET_H
#define TEMPHUMPRESSURENET_H

#include <Arduino.h>
#include <THPInterface.h>
#include <NetComponent.h>

class THPNet : public NetComponent {

public:
    THPNet(SmartNet *n, uint8_t sp, uint8_t max, THPInterface *thp) : NetComponent(n, sp, max) {
        tempHumPressure = thp;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    THPInterface *tempHumPressure;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};
};

#endif //TEMPHUMPRESSURENET_H
