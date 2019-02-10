#ifndef TEMPNET_H
#define TEMPNET_H

#include <Arduino.h>
#include <Temp.h>
#include <NetComponent.h>
#include <Net.h>

class TempNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    TempNet(uint16_t p, SmartNet *n, Temp *t);

    virtual ~TempNet() {};

    void sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

protected:
    Temp *temperature;
};

#endif //TEMPNET_H
