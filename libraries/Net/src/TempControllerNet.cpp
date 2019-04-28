#include "TempControllerNet.h"

TempControllerNet::TempControllerNet(uint16_t p, SmartNet *n, TempController *tCtrl) : NetComponent(p, n) {
    tempCtrl = tCtrl;
}

void TempControllerNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
//    p.data = info;

    n->sendData(p);
}
