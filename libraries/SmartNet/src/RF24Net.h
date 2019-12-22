#ifndef RF24NET_H
#define RF24NET_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <RadioInterface.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SmartNet.h>
#include <Format.h>

#define RF24_DEFAULT_CE 8
#define RF24_DEFAULT_CSN 9
#define RF24_CHANNEL 46

class RF24Net : public RadioInterface {
    using RadioInterface::RadioInterface;

protected:
    RF24 &radio;
    RF24Network *network;
    void (*receiveCallback)(Packet *);

public:

    RF24Net(SmartNet *net, uint16_t node, RF24& radio);

    void sendData(Packet *p) override;

    void receiveData(Packet *p) override;

    void onReceiveFunc(void(*callback)(Packet *));

    void tick();
};

#endif //RF24NET_H
