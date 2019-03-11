#include "InfoNet.h"

InfoNet::InfoNet(uint16_t p, SmartNet *n) : NetComponent(p, n) {

}

void InfoNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = info;

    n->sendData(p);
}

void InfoNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd, long value) {
    info = value;
    sendCommandData(n, r, rp, cmd);
}
