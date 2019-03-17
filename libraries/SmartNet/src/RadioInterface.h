#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <Net.h>

class RadioInterface {
public:
    virtual ~RadioInterface() {};

    virtual void sendData(Packet &p) = 0;

    virtual bool setup() = 0;

    bool isEnabled();

protected:

    bool enabled = false;
};

#endif //RADIO_H