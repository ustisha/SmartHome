#include "LightNet.h"

LightNet::LightNet(uint16_t p, SmartNet *n, LightSensor *ls) : NetComponent(p, n) {
    lightSensor = ls;
}

void LightNet::sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) {
    lightSensor->read();

    Packet p;
    p.sender = net->getSender();
    p.sp = sport;
    p.receiver = r;
    p.rp = rp;
    p.cmd = cmd;
    p.data = round(lightSensor->get() * 100);

    n->sendData(p);
}
