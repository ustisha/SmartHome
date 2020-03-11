#ifndef TEMPHUMPRESSURE_H
#define TEMPHUMPRESSURE_H

#include <Arduino.h>
#include <THInterface.h>

class THPInterface : public THInterface {

public:

    float getPressure() {
        return pressure;
    }

protected:

    float pressure{};

};

#endif //TEMPHUMPRESSURE_H
