#include "VccNet.h"

VccNet::VccNet(SmartNet *n, uint8_t sp, uint8_t max, Vcc *v) : NetComponent(n, sp, max) {
    vcc = v;
}

void VccNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);
    p.setData(round(vcc->Read_Volts() * 100));

    n->sendData(&p);
}
