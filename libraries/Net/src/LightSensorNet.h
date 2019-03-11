#ifndef LIGHTNET_H
#define LIGHTNET_H

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <LightSensorInterface.h>
#include <SmartNet.h>

class LightSensorNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    LightSensorNet(uint16_t p, SmartNet *n, LightSensorInterface *ls);

    virtual ~LightSensorNet() {};

    void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

protected:
    LightSensorInterface *lightSensor;
};

#endif //LIGHTNET_H
