#ifndef TEMPHUM_H
#define TEMPHUM_H

#include <Arduino.h>
#include <TInterface.h>

class THInterface : public TInterface {

public:

    float getHumidity() {
        return hum;
    }

protected:
    float hum{};
};

#endif //TEMPHUM_H
