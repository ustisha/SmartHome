#ifndef ONEWIRETEMPERATURE_H
#define ONEWIRETEMPERATURE_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <TInterface.h>
#include <DallasTemperature.h>

class DS18B20Adapter : public TInterface {
    using TInterface::TInterface;
public:
    DS18B20Adapter(OneWire *oneWire, byte idx);

    void read() override;
    
protected:
    DallasTemperature *sensors;
    byte addressIdx;
};

#endif //ONEWIRETEMPERATURE_H
