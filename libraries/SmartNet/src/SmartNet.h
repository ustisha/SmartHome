#ifndef SMARTNET_H
#define SMARTNET_H

#include <Arduino.h>
#include <DebugLog.h>
#include <LoraNet.h>

class SmartNet {
    const static uint8_t MAX = 10;
//    const static uint8_t CHANNELS = 2;

public:
    SmartNet(uint16_t s);

    void tick();

    uint8_t getSender();

protected:

//    int getIndex();

//    NetComponent **components = new NetComponent *[MAX];
    uint8_t sender;
};

#endif //SMARTNET_H
