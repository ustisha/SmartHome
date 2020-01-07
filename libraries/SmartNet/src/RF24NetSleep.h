#ifndef RF24NETSLEEP_H
#define RF24NETSLEEP_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <RF24Net.h>

class RF24NetSleep : public RF24Net {

public:

    using RF24Net::RF24Net;

    void sendData(Packet *p) override;

    using RF24Net::receiveData;
};

#endif //RF24NETSLEEP_H
