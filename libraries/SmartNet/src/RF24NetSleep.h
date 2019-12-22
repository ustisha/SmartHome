#ifndef RF24NETSLEEP_H
#define RF24NETSLEEP_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <RF24Net.h>

class RF24NetSleep : public RF24Net {
    using RF24Net::RF24Net;

public:

    void sendData(Packet *p) override;

    void receiveData(Packet *p) override;

};

#endif //RF24NETSLEEP_H
