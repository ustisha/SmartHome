#include "THPNet.h"

THPNet::THPNet(uint16_t p, SmartNet *n, THPInterface *thp) : NetComponent(p, n) {
    tempHumPressure = thp;
}

void THPNet::sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;

    if (cmd == CMD_TEMPERATURE) {
        p.data = long(tempHumPressure->get() * 100);
    } else if (cmd == CMD_HUMIDITY) {
        p.data = long(tempHumPressure->getHumidity() * 100);
    } else if (cmd == CMD_PRESSURE) {
        p.data = long(tempHumPressure->getPressure() * 100);
    }
    n->sendData(p);
}
