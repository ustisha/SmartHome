#include "ValueIntNet.h"

ValueIntNet::ValueIntNet(SmartNet *n, uint8_t sp, uint8_t max, ValueIntInterface *vInt) : NetComponent(n, sp, max) {
    valueInt = vInt;
}

void ValueIntNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);
    p.setData(valueInt->get());

    n->sendData(&p);
}
