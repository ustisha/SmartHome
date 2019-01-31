#ifndef SMARTHOME_ONEWIRETEMPERATURENET_H
#define SMARTHOME_ONEWIRETEMPERATURENET_H

#include <Arduino.h>
#include <Temperature.h>
#include <NetComponent.h>

class TemperatureNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    TemperatureNet(uint16_t p, Temperature *t);

protected:
    Temperature *temperature;
};

#endif //SMARTHOME_ONEWIRETEMPERATURENET_H
