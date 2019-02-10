#ifndef TEMPHUMPRESSURENET_H
#define TEMPHUMPRESSURENET_H

#include <Arduino.h>
#include <TempHumPressure.h>
#include <NetComponent.h>
#include <Net.h>

class TempHumPressureNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    TempHumPressureNet(uint16_t p, SmartNet *n, TempHumPressure *thp);

    virtual ~TempHumPressureNet() {};

    void sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

protected:
    TempHumPressure *tempHumPressure;
};

#endif //TEMPHUMPRESSURENET_H
