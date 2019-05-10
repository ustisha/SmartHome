#include "SmartNet.h"

SmartNet::SmartNet(uint16_t s) {
    sender = s;
}

int SmartNet::getIndex() {
    uint8_t i = 0;
    do {
        if (!components[i].enabled) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

uint8_t SmartNet::getSender() {
    return sender;
}

void SmartNet::addNetComponent(NetComponent *nc) {
    int idx = getIndex();
    if (idx != -1) {
        components[idx].enabled = true;
        components[idx].netComponent = nc;
    }
}

void SmartNet::commandReceived(Packet packet) {
    for (int i = 0; i < MAX; i++) {
        if(components[i].enabled) {
            components[i].netComponent->receiveHandle(packet.sp, packet.cmd, packet.data);
        }
    }
}

