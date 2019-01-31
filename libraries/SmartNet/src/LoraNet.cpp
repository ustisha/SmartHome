#include "LoraNet.h"

LoraNet::LoraNet(int ss, int reset, int dio0) {
    LoRa.setPins(ss, reset, dio0);
}

bool LoraNet::setup() {
    LoRa.begin(433E6);
    LoRa.setTxPower(10);
    LoRa.setSpreadingFactor(8);
    LoRa.enableCrc();
    LoRa.idle();

    return true;
}

void LoraNet::sendData() {
    LoRa.write(1);
}

