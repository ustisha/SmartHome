#ifndef VCCNET_H
#define VCCNET_H

#include <Arduino.h>
#include <SmartNet.h>
#include <NetComponent.h>
#include <Vcc.h>

class VccNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    VccNet(SmartNet *n, uint8_t sp, uint8_t max, Vcc *v) : NetComponent(n, sp, max) {
        vcc = v;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    Vcc *vcc;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};
};


#endif //VCCNET_H
