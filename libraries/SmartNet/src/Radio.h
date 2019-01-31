#ifndef SMARTHOME_RADIO_H
#define SMARTHOME_RADIO_H

#include <Arduino.h>

class Radio {
public:
    virtual void sendData() = 0;

    virtual bool setup() = 0;

protected:
};

#endif //SMARTHOME_RADIO_H
