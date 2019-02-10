#ifndef ONEWIRETEMPERATURE_H
#define ONEWIRETEMPERATURE_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Temp.h>
#include <DallasTemperature.h>
#include <NetComponent.h>

class OneWireTemp : public Temp {
    using Temp::Temp;
public:
    OneWireTemp(OneWire *oneWire, byte idx);

    void read() override;
    
protected:
    DallasTemperature *sensors;
    byte addressIdx;
};

#endif //ONEWIRETEMPERATURE_H
