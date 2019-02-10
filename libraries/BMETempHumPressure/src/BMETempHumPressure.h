#ifndef BMETEMPHUMPRESSURE_H
#define BMETEMPHUMPRESSURE_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <TempHumPressure.h>
#include <BME280.hpp>

class BMETempHumPressure : public TempHumPressure {
    using TempHumPressure::TempHumPressure;
public:
    BMETempHumPressure(BME280 *bme, uint8_t address);

    void read() override;

protected:

};

#endif //BMETEMPHUMPRESSURE_H
