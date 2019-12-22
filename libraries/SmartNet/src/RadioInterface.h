#ifndef RADIO_INTERFACE_H
#define RADIO_INTERFACE_H

class SmartNet;

#include <Arduino.h>
#include <Net.h>

class RadioInterface {
public:

    RadioInterface(SmartNet *net);

    virtual void sendData(Packet *p) = 0;

    virtual void receiveData(Packet *p) = 0;

    int getStatus();

protected:

    int status;
    SmartNet *smartNet;
};

#endif //RADIO_INTERFACE_H
