#include "InfoNet.h"

void InfoNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);
    p.setData(info);

    n->sendData(&p);
}

void InfoNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd, long value) {
    info = value;
    sendCommandData(n, r, rp, cmd);
}
