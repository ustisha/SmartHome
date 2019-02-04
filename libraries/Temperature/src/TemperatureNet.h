#ifndef SMARTHOME_ONEWIRETEMPERATURENET_H
#define SMARTHOME_ONEWIRETEMPERATURENET_H

#include <Arduino.h>
#include <Temperature.h>
#include <NetComponent.h>
#include <Networking.h>

class TemperatureNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    TemperatureNet(uint16_t p, SmartNet *n, Temperature *t);

    void sendCommandData(uint16_t r, uint16_t rp, uint8_t cmd) override;

protected:
    Temperature *temperature;
};

#endif //SMARTHOME_ONEWIRETEMPERATURENET_H
