#ifndef LIGHTNET_H
#define LIGHTNET_H

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <LightSensor.h>
#include <SmartNet.h>

class LightNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    LightNet(uint16_t p, SmartNet *n, LightSensor *ls);

    virtual ~LightNet() {};

    void sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

protected:
    LightSensor *lightSensor;
};

#endif //LIGHTNET_H
