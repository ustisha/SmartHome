#include "NetComponent.h"

NetComponent::NetComponent(uint16_t sp, SmartNet *n) {
    sport = sp;
    net = n;
}

void NetComponent::addReceiver(uint16_t r, uint16_t rp, uint16_t c, uint16_t t) {
    int i = getIndex();
    if (i > -1) {
        rcvr[i].receiver = r;
        rcvr[i].rport = rp;
        rcvr[i].timeout = t;
        rcvr[i].cmd = c;
        rcvr[i].last = millis();
        LOG("[NetComponent] Receiver added to network. Idx: ");
        LOGVLN(i);
        LOG(" * Receiver: ");
        LOGVLN(r);
        LOG(" * Receiver port: ");
        LOGVLN(rp);
        LOG(" * Command: ");
        LOGVLN(c);
    }
}

int NetComponent::getIndex() {
    uint8_t i = 0;
    do {
        if (rcvr[i].receiver == NULL) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

void NetComponent::tick() {
    unsigned long m = millis();
    for (int i = 0; i <= MAX; i++) {
        if (rcvr[i].receiver != NULL && m >= (rcvr[i].last + rcvr[i].timeout)) {
            rcvr[i].last += rcvr[i].timeout;
            LOG("[NetComponent] Send data by timeout. Idx: ");
            LOGVLN(i);
            sendCommandData(rcvr[i].receiver, rcvr[i].rport, rcvr[i].cmd);
        }
    }
}
