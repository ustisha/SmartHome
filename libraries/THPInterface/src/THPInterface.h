#ifndef TEMPHUMPRESSURE_H
#define TEMPHUMPRESSURE_H

#include <Arduino.h>
#include <THInterface.h>

class THPInterface : public THInterface {
    using THInterface::THInterface;

public:

    float getPressure();

protected:

    float pressure;

};

#endif //TEMPHUMPRESSURE_H
