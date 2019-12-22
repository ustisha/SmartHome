#include "LightSensorNet.h"

LightSensorNet::LightSensorNet(SmartNet *n, uint8_t sp, uint8_t max, LightSensorInterface *ls) : NetComponent(n, sp, max) {
    lightSensor = ls;
}

void LightSensorNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    lightSensor->read();

    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);
    p.setData(round(lightSensor->get() * 100));

    n->sendData(&p);
}
