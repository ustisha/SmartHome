#include "TempNet.h"

TempNet::TempNet(uint16_t p, SmartNet *n, Temp *t) : NetComponent(p, n) {
    temperature = t;
}

void TempNet::sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = round(temperature->get() * 100);

    n->sendData(p);
}