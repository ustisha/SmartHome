#ifndef TEMPHUM_H
#define TEMPHUM_H

#include <Arduino.h>
#include <Temp.h>

class TempHum : public Temp {
    using Temp::Temp;
public:

    float getHumidity();

protected:
    float hum;
};

#endif //TEMPHUM_H
