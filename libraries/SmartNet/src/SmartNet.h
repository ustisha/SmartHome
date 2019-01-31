
#ifndef OUTSIDE_TEMP_SMARTNET_H
#define OUTSIDE_TEMP_SMARTNET_H

class NetComponent;

#include <Arduino.h>
#include "NetComponent.h"
#include <Radio.h>

union Int {
    int i = 0;
    uint8_t b[sizeof(int)];
};

union Float {
    float f = 0;
    uint8_t b[sizeof(float)];
};

union Byte {
    byte bt = 0;
    uint8_t b[sizeof(byte)];
};

const int GATEWAY = 1;
const int GATEWAY_HTTP_HANDLER = 10001;

const int OUTSIDE_TEMP = 2;
const int OUTSIDE_TEMP_18B20 = 20001;

struct NetV1 {
    uint16_t sender;
    uint16_t sp;
    uint16_t receiver;
    uint16_t rp;
    uint8_t cmd;
    int data;
};

class SmartNet {
    const static uint8_t MAX = 10;
    const static uint8_t CHANNELS = 2;

public:
    SmartNet(uint16_t s);

    void addRadioChannel(Radio &r, byte idx = 0);

    void addNetComponent(NetComponent *cmp);

    void tick();

protected:

    int getIndex();

    Radio *channels[CHANNELS];
    NetComponent *components[MAX];
    uint16_t sender;
};

#endif //OUTSIDE_TEMP_SMARTNET_H
