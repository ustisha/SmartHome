#ifndef BHLIGHT_H
#define BHLIGHT_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <LightSensor.h>
#include <BH1750.h>

class BHLight : public LightSensor {
    using LightSensor::LightSensor;
public:

    explicit BHLight();

    void read() override;

protected:
    BH1750 *bh;

};

#endif //BHLIGHT_H
