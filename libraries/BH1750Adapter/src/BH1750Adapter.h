#ifndef BHLIGHT_H
#define BHLIGHT_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <BH1750.h>
#include <LightSensorInterface.h>

class BH1750Adapter : public LightSensorInterface {
    typedef BH1750::Mode BHMode;
public:
    explicit BH1750Adapter(BHMode mode);

    void read() override;

    auto getStatus() -> int;

protected:
    BH1750 *bh;
    int status = 0;
};

#endif //BHLIGHT_H
