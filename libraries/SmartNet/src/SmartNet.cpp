#include "SmartNet.h"

SmartNet::SmartNet(uint16_t s) {
    sender = s;
}

void SmartNet::addRadioChannel(Radio &r, byte idx) {
    channels[idx] = &r;
}

void SmartNet::tick() {
    for (int i = 0; i < 2; ++i) {

    }
}

void SmartNet::addNetComponent(NetComponent *cmp) {
    int i = getIndex();
    if (i > -1) {
        components[i] = cmp;
    }
}

int SmartNet::getIndex() {
    uint8_t i = 0;
    do {
        if (components[i] == NULL) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

