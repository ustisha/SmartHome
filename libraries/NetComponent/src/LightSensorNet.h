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
    LightSensorNet(SmartNet *n, uint8_t sp, uint8_t max, LightSensorInterface *ls);

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    LightSensorInterface *lightSensor;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};
};

#endif //LIGHTNET_H
