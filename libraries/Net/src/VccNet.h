#ifndef VCCNET_H
#define VCCNET_H

#include <Arduino.h>
#include <SmartNet.h>
#include <NetComponent.h>
#include <Vcc.h>

class VccNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    VccNet(uint16_t p, SmartNet *n, Vcc *v);

    virtual ~VccNet() {};

    void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

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
