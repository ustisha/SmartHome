#ifndef TEMPNET_H
#define TEMPNET_H

#include <Arduino.h>
#include <TInterface.h>
#include <NetComponent.h>

class TNet : public NetComponent {

public:
    TNet(SmartNet *n, uint8_t sp, uint8_t max, TInterface *t) : NetComponent(n, sp, max) {
        temperature = t;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    TInterface *temperature;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};

};

#endif //TEMPNET_H
