#ifndef OUTSIDE_TEMP_TEMPERATURE_H
#define OUTSIDE_TEMP_TEMPERATURE_H

#include <Arduino.h>

class Temperature {
public:
    Temperature();

    virtual void read() = 0;

    virtual float get() = 0;

protected:
    float temp = -1;
};

#endif //OUTSIDE_TEMP_TEMPERATURE_H
