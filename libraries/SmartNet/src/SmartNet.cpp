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

int SmartNet::getIndex() {
    uint8_t i = 0;
    do {
        if (components[i] == NULL) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

void SmartNet::sendPacket(Packet p) {
    for (int i = 0; i < MAX; ++i) {
        channels[i]->sendData(p);
    }
}

uint16_t SmartNet::getSender() {
    return sender;
}

