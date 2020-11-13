#ifndef LIGHTNET_H
#define LIGHTNET_H

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <LightSensorInterface.h>

class LightSensorNet : public NetComponent {

public:
    LightSensorNet(SmartNet *n, uint8_t sp, uint8_t max, LightSensorInterface *ls) : NetComponent(n, sp, max) {
        lightSensor = ls;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    LightSensorInterface *lightSensor;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(Packet *p) override {};
};

#endif //LIGHTNET_H
