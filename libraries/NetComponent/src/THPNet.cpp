#include "THPNet.h"

THPNet::THPNet(SmartNet *n, uint8_t sp, uint8_t max, THPInterface *thp) : NetComponent(n, sp, max) {
    tempHumPressure = thp;
}

void THPNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);

    if (cmd == CMD_TEMPERATURE) {
        p.setData((long) (tempHumPressure->get() * 100));
    } else if (cmd == CMD_HUMIDITY) {
        p.setData((long) (tempHumPressure->getHumidity() * 100));
    } else if (cmd == CMD_PRESSURE) {
        p.setData((long) (tempHumPressure->getPressure() * 100));
    }
    n->sendData(&p);
}
