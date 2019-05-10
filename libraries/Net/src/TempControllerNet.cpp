#include "TempControllerNet.h"

TempControllerNet::TempControllerNet(uint16_t p, SmartNet *n, TempController *tCtrl) : NetComponent(p, n) {
    tempCtrl = tCtrl;
}

void TempControllerNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p{};
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;

    if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        p.data = tempCtrl->getRelayState(cmd - CMD_RELAY_00);
    } else if (cmd == CMD_MODE) {
        p.data = tempCtrl->getMode();
    }

    n->sendData(p);
}

void TempControllerNet::receiveCommandData(uint8_t cmd, long data) {
    if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        tempCtrl->setRelayState(cmd - CMD_RELAY_00, data);
    } else if (cmd == CMD_MODE) {
        tempCtrl->setMode(data);
    }
}
