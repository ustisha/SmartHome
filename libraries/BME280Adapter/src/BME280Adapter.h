#ifndef BMETEMPHUMPRESSURE_H
#define BMETEMPHUMPRESSURE_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <THPInterface.h>
#include <Wire.h>
#include <BME280.h>

class BME280Adapter : public THPInterface {
    using THPInterface::THPInterface;
public:
    BME280Adapter(TwoWire &wire, uint8_t address);

    void read() override;

protected:
    BME280 *bme;
};

#endif //BMETEMPHUMPRESSURE_H
