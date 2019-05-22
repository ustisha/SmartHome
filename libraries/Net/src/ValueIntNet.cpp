#include "ValueIntNet.h"

ValueIntNet::ValueIntNet(uint16_t p, SmartNet *n, ValueIntInterface *vInt) : NetComponent(p, n) {
    valueInt = vInt;
}

void ValueIntNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = valueInt->get();
    n->sendData(p);
}
