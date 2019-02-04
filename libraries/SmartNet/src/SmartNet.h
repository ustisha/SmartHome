#ifndef OUTSIDE_TEMP_SMARTNET_H
#define OUTSIDE_TEMP_SMARTNET_H

class NetComponent;

#include <Arduino.h>
//#include <NetComponent.h>
#include <Radio.h>

const int GATEWAY = 1;
const int GATEWAY_HTTP_HANDLER = 1001;

const int OUTSIDE_TEMP = 2;
const int OUTSIDE_TEMP_18B20 = 2001;
const int OUTSIDE_TEMP_CMD_TEMPERATURE = 200101;

class SmartNet {
    const static uint8_t MAX = 10;
    const static uint8_t CHANNELS = 2;

public:
    SmartNet(uint16_t s);

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
