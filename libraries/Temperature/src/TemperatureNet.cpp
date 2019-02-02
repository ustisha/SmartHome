#include "TemperatureNet.h"

TemperatureNet::TemperatureNet(uint16_t p, SmartNet *n, Temperature *t) : NetComponent(p, n) {
    temperature = t;
}

void TemperatureNet::sendCommandData(uint16_t r, uint16_t rp, uint8_t cmd) {
    temperature->read();

    Packet p;
    p.sender.i = net.getSender();
    p.sp.i = sport;
    p.receiver.i = r;
    p.rp.i = rp;
    p.cmd = cmd;
    p.data.i = temperature->get();
}