#ifndef BHLIGHT_H
#define BHLIGHT_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <BH1750.h>
#include <LightSensorInterface.h>

class BH1750Adapter : public LightSensorInterface {
    using LightSensorInterface::LightSensorInterface;
public:

    explicit BH1750Adapter();

    void read() override;

    int getStatus();

protected:
    BH1750 *bh;
    int status = 0;
};

#endif //BHLIGHT_H
