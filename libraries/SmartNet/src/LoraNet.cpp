#include "LoraNet.h"

LoraNet::LoraNet(int ss, int reset, int dio0) {
    LoRa.setPins(ss, reset, dio0);
    enabled = true;
}

bool LoraNet::setup() {
    LoRa.begin(433E6);
    LoRa.setTxPower(10);
    LoRa.setSpreadingFactor(8);
    LoRa.enableCrc();
    LoRa.idle();

    return true;
}

void LoraNet::sendData(Packet p) {
    LoRa.beginPacket();

    LoRa.write(p.sender.b[0]);
    LoRa.write(p.sender.b[1]);
    LoRa.write(p.sp.b[0]);
    LoRa.write(p.sp.b[1]);

    LoRa.write(p.receiver.b[0]);
    LoRa.write(p.receiver.b[1]);
    LoRa.write(p.rp.b[0]);
    LoRa.write(p.rp.b[1]);

    LoRa.write(p.cmd.b[0]);
    LoRa.write(p.cmd.b[1]);

    LoRa.write(p.data.b[0]);
    LoRa.write(p.data.b[1]);
    LoRa.write(p.data.b[2]);
    LoRa.write(p.data.b[3]);

    LoRa.endPacket();
    LoRa.receive();
}

