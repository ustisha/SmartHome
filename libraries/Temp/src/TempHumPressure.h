#ifndef TEMPHUMPRESSURE_H
#define TEMPHUMPRESSURE_H

#include <Arduino.h>
#include <TempHum.h>

class TempHumPressure : public TempHum {
    using TempHum::TempHum;

public:

    float getPressure();

protected:

    float pressure;

};

#endif //TEMPHUMPRESSURE_H
