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
};

#endif //TEMPHUMPRESSURENET_H
