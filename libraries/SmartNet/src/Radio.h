#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <Net.h>

class Radio {
public:
    virtual ~Radio() {};

    virtual void sendData(Packet &p) = 0;

    virtual bool setup() = 0;

    bool isEnabled();

protected:

    bool enabled = false;
};

#endif //RADIO_H
