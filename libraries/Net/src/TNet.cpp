#include "TNet.h"

TNet::TNet(uint16_t p, SmartNet *n, TInterface *t) : NetComponent(p, n) {
    temperature = t;
}

void TNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = round(temperature->get() * 100);

    n->sendData(p);
}