#ifndef OUTSIDE_TEMP_SMARTNET_H
#define OUTSIDE_TEMP_SMARTNET_H

class NetComponent;

#include <Arduino.h>
#include <DebugLog.h>
#include <Radio.h>

class SmartNet {
    const static uint8_t MAX = 10;
    const static uint8_t CHANNELS = 2;

public:
    explicit SmartNet(uint16_t s);

    void addRadioChannel(Radio &r, byte idx = 0);

    void sendPacket(Packet p);

    void tick();

    uint16_t getSender();

protected:

    int getIndex();

    Radio *channels[CHANNELS];
    NetComponent *components[MAX];
    uint16_t sender;
};

#endif //OUTSIDE_TEMP_SMARTNET_H
