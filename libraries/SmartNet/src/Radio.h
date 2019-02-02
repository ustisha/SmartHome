#ifndef SMARTHOME_RADIO_H
#define SMARTHOME_RADIO_H

#include <Arduino.h>
#include <Networking.h>

class Radio {
public:
    virtual void sendData(Packet p) = 0;

    virtual bool setup() = 0;

protected:
};

#endif //SMARTHOME_RADIO_H
