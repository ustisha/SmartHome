#include "LoraNetEE.h"

LoraNetEE::LoraNetEE(uint8_t ss, uint8_t reset, uint8_t dio0) : LoraNet(ss, reset, dio0) {
    LoRa.sleep();
}

void LoraNetEE::sendData(Packet &p) {
    LoraNet::sendData(p);
    LoRa.sleep();
}
