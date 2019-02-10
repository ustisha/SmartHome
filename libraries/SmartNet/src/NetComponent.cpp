#include "NetComponent.h"

NetComponent::NetComponent(uint16_t sp, SmartNet *n) {
    sport = sp;
    net = n;
}

/**
 * Attach receiver data to network component.
 * @param n Network carrier.
 * @param r Receiver id.
 * @param rp Receiver port.
 * @param c Command to send.
 * @param t Timeout in seconds.
 */
void NetComponent::addReceiver(Radio *n, uint8_t r, uint16_t rp, uint8_t c, uint16_t t) {
    int i = getIndex();
    IF_SERIAL_DEBUG(printf_P(PSTR("[NetComponent::addReceiver] Idx: %d\n"), i));
    if (i > -1) {
        rcvr[i].network = n;
        rcvr[i].receiver = r;
        rcvr[i].rport = rp;
        rcvr[i].timeout = t * 1000;
        rcvr[i].cmd = c;
        rcvr[i].last = millis();
    }
}

byte NetComponent::getIndex() {
    uint8_t i = 0;
    do {
        if (rcvr[i].receiver == 0) {
            return i;
        }
    } while (i++ < NetComponent::MAX);
    return -1;
}

void NetComponent::tick() {
    unsigned long m = millis();
    for (int i = 0; i < NetComponent::MAX; i++) {
        if (rcvr[i].receiver != 0 && m >= (rcvr[i].last + rcvr[i].timeout)) {
            rcvr[i].last += rcvr[i].timeout;
            IF_SERIAL_DEBUG(printf_P(PSTR("[NetComponent::tick] By timeout. Idx: %d\n"), i));
            sendCommandData(rcvr[i].network, rcvr[i].receiver, rcvr[i].rport, rcvr[i].cmd);
        }
    }
}
