#ifndef OUTSIDE_TEMP_ONEWIRETEMPERATURE_H
#define OUTSIDE_TEMP_ONEWIRETEMPERATURE_H

#include <Arduino.h>
#include <DebugLog.h>
#include <Temperature.h>
#include <DallasTemperature.h>
#include <NetComponent.h>

class OneWireTemperature : public Temperature {
    using Temperature::Temperature;
public:
    OneWireTemperature(OneWire *oneWire, byte idx);

    void read() override;

    float get() override;

protected:
    DallasTemperature *sensors;
    byte addressIdx;
};

#endif //OUTS{}IDE_TEMP_ONEWIRETEMPERATURE_H
