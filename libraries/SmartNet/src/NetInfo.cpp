#include "NetInfo.h"

NetInfo::NetInfo(uint16_t p, SmartNet *n) : NetComponent(p, n) {

}

void NetInfo::sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = info;

    n->sendData(p);
}

void NetInfo::sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd, long value) {
    info = value;
    sendCommandData(n, r, rp, cmd);
}
