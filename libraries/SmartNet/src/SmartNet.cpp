#include "SmartNet.h"

SmartNet::SmartNet(uint16_t s, uint8_t max) : components(new NetComponents[max]{}) {
    sender = s;
    maxCmp = max;
}

void SmartNet::addNetComponent(NetComponent *nc) {
    if (i < maxCmp) {
        components[i].enabled = true;
        components[i].netComponent = nc;
        IF_SERIAL_DEBUG(printf_P(PSTR("[SmartNet::addNetComponent] Idx: %d\n"), i));
        i++;
    } else {
        IF_SERIAL_DEBUG(PSTR("[SmartNet::addNetComponent] Component limit reached\n"));
    }
}

void SmartNet::commandReceived(Packet *p) {
    for (int i = 0; i < maxCmp; i++) {
        if (components[i].enabled) {
            components[i].netComponent->receiveHandle(p->getReceiverPort(), p->getCommand(), p->getData());
        }
    }
}

