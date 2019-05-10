#ifndef TEMPHUMPRESSURENET_H
#define TEMPHUMPRESSURENET_H

#include <Arduino.h>
#include <THPInterface.h>
#include <NetComponent.h>

class THPNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    THPNet(uint16_t p, SmartNet *n, THPInterface *thp);

    virtual ~THPNet() {};

    void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

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
