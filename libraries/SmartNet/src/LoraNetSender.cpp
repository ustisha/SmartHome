#include "LoraNetSender.h"

LoraNetSender::LoraNetSender(uint8_t ss, uint8_t reset, uint8_t dio0) : LoraNet(ss, reset, dio0) {
    LoRa.sleep();
    delayMicroseconds(100);
}

void LoraNetSender::sendData(Packet &p) {
    LoraNet::sendData(p);
    LoRa.sleep();
    delayMicroseconds(100);
}
