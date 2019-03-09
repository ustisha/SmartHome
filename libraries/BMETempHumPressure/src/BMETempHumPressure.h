#ifndef BMETEMPHUMPRESSURE_H
#define BMETEMPHUMPRESSURE_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <TempHumPressure.h>
#include <BME280.h>

class BMETempHumPressure : public TempHumPressure {
    using TempHumPressure::TempHumPressure;
public:
    BMETempHumPressure(TwoWire &wire, uint8_t address);

    void read() override;

protected:
    BME280 *bme;
};

#endif //BMETEMPHUMPRESSURE_H
