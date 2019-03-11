#include "VccNet.h"

VccNet::VccNet(uint16_t p, SmartNet *n, Vcc *v) : NetComponent(p, n) {
    vcc = v;
}

void VccNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = round(vcc->Read_Volts() * 100);

    n->sendData(p);
}
