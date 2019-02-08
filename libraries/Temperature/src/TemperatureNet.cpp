#include "TemperatureNet.h"

TemperatureNet::TemperatureNet(uint16_t p, SmartNet *n, Temperature *t) : NetComponent(p, n) {
    temperature = t;
}

void TemperatureNet::sendCommandData(uint16_t r, uint16_t rp, uint16_t cmd) {
    temperature->read();

    Packet p;
    p.sender.i = net->getSender();
    p.sp.i = sport;
    p.receiver.i = r;
    p.rp.i = rp;
    p.cmd.i = cmd;
    p.data.i = int32_t (temperature->get() * 100);

    net->sendPacket(p);
 }