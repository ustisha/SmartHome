#ifndef TEMP_H
#define TEMP_H

#include <Arduino.h>

class Temp {
public:
    Temp();

    virtual void read() = 0;

    float get();

    int getStatus();

protected:
    float temp = -1;
    int status = -1;
};

#endif //TEMP_H
