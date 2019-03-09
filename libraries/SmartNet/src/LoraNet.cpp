#include "LoraNet.h"

LoraNet::LoraNet(uint8_t ss, uint8_t reset, uint8_t dio0) {
    LoRa.setPins(ss, reset, dio0);
    enabled = setup();
    LoRa.receive();

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet] Initialized\n")));
}

bool LoraNet::setup() {
    LoRa.begin(433E6);
    LoRa.setTxPower(10);
    LoRa.setSpreadingFactor(8);
    LoRa.enableCrc();
    LoRa.idle();

    return true;
}

void LoraNet::sendData(Packet &p) {

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[LoraNet::sendData] Sender: %i, Sport: %u, Receiver: %i, Rport: %u, Cmd: %i, Data: %ld\n"),
                     p.sender, p.sp, p.receiver, p.rp, p.cmd, p.data));

    unsigned long start, m;
    start = millis();
    while (LoRa.beginPacket() == 0) {
        m = millis();
        if ((m - start) > WAIT_TRANSMITTING) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::sendData] Wait transmitting failed\n")));
            break;
        }
        delayMicroseconds(100);
    }
    // @todo Send immediately after previous send without delay not working.
    delay(10);

    UInt sp(p.sp);
    UInt rp(p.rp);
    Long data(p.data);

    LoRa.write(p.sender);
    LoRa.write(sp.b[0]);
    LoRa.write(sp.b[1]);

    LoRa.write(p.receiver);
    LoRa.write(rp.b[0]);
    LoRa.write(rp.b[1]);

    LoRa.write(p.cmd);

    LoRa.write(data.b[0]);
    LoRa.write(data.b[1]);
    LoRa.write(data.b[2]);
    LoRa.write(data.b[3]);

    int status;
    status = LoRa.endPacket(true);
    LoRa.receive();

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::sendData] Sent. Status: %d\n"), status));
}