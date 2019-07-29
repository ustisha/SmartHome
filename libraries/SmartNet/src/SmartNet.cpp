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
    } while (i++ <= SmartNet::MAX);
    return -1;
}

uint8_t SmartNet::getSender() {
    return sender;
}

void SmartNet::addNetComponent(NetComponent *nc) {
    int idx = getIndex();
    IF_SERIAL_DEBUG(printf_P(PSTR("[SmartNet::addNetComponent] Idx: %d\n"), idx));
    if (idx != -1) {
        components[idx].enabled = true;
        components[idx].netComponent = nc;
    }
}

void SmartNet::commandReceived(Packet packet) {
    for (int i = 0; i < SmartNet::MAX; i++) {
        if (components[i].enabled) {
            components[i].netComponent->receiveHandle(packet.rp, packet.cmd, packet.data);
        }
    }
}

