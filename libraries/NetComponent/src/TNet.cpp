#include "TNet.h"

TNet::TNet(SmartNet *n, uint8_t sp, uint8_t max, TInterface *t) : NetComponent(n, sp, max) {
    temperature = t;
}

void TNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);
    p.setData(round(temperature->get() * 100));

    n->sendData(&p);
}