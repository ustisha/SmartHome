#include "NetComponent.h"

NetComponent::NetComponent(SmartNet *n, uint8_t sp, uint8_t max) : rcvr(new R[max]{}) {
    sport = sp;
    net = n;
    sleepTime = 0;
    i = 0;
    maxCmp = max;
    net->addNetComponent(this);
}

/**
 * Attach receiver data to network component.
 * @param n Network carrier.
 * @param r Receiver id.
 * @param rp Receiver port.
 * @param c Command to send.
 * @param t Timeout in seconds.
 */
void NetComponent::addReceiver(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t c, float t) {
    if (i < maxCmp) {
        rcvr[i].network = n;
        rcvr[i].enabled = true;
        rcvr[i].receiver = r;
        rcvr[i].rport = rp;
        rcvr[i].timeout = uint32_t(t * 1000);
        rcvr[i].cmd = c;
        rcvr[i].last = millis();
        IF_SERIAL_DEBUG(printf_P(PSTR("[NetComponent::addReceiver] Idx: %d\n"), i));
        i++;
    }
}

void NetComponent::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    for (int i = 0; i < maxCmp; i++) {
        if (rcvr[i].enabled && (m >= (rcvr[i].last + rcvr[i].timeout) || rcvr[i].last < rcvr[i].timeout)) {
            rcvr[i].last += rcvr[i].timeout;
            IF_SERIAL_DEBUG(printf_P(PSTR("[NetComponent::tick] By timeout. Idx: %d\n"), i));
            sendCommandData(rcvr[i].network, rcvr[i].receiver, rcvr[i].rport, rcvr[i].cmd);
        }
        // @todo Проверить как работает таймер при переполнении millis();
        /*if (rcvr[i].receiver != 0 && millis() < rcvr[i].last) {
            rcvr[i].last = m;
        }*/
    }
}

void NetComponent::receiveHandle(uint16_t rp, uint8_t cmd, long data) {
    if (rp == this->sport) {
        receiveCommandData(cmd, data);
    }
}
