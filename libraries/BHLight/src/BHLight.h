#ifndef BHLIGHT_H
#define BHLIGHT_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <LightSensor.h>
#include <BH1750.h>

class BHLight : public LightSensor {
    using LightSensor::LightSensor;
public:

    explicit BHLight();

    void read() override;

    int getStatus();

protected:
    BH1750 *bh;
    int status = 0;
};

#endif //BHLIGHT_H
