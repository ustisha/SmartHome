#ifndef VALUEINTNET_H
#define VALUEINTNET_H

#include <Arduino.h>
#include <ValueIntInterface.h>
#include <NetComponent.h>

class ValueIntNet : public NetComponent {

public:
    ValueIntNet(SmartNet *n, uint8_t sp, uint8_t max, ValueIntInterface *vInt) : NetComponent(n, sp, max) {
        valueInt = vInt;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    ValueIntInterface *valueInt;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(Packet *p) override {};
};

#endif //VALUEINTNET_H
