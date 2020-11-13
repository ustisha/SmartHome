#ifndef THNET_H
#define THNET_H

#include <Arduino.h>
#include <THInterface.h>
#include <NetComponent.h>

class THNet : public NetComponent {

public:
    THNet(SmartNet *n, uint8_t sp, uint8_t max, THInterface *th) : NetComponent(n, sp, max) {
        tempHum = th;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    THInterface *tempHum;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(Packet *p) override {};
};

#endif //THNET_H
