#include "LoraNet.h"

LoraNet::LoraNet(uint8_t ss, uint8_t reset, uint8_t dio0) {
    LoRa.setPins(ss, reset, dio0);
    enabled = setup();

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet] Initialize status: %d\n"), (int) enabled));
}

bool LoraNet::setup() {
    int status = LoRa.begin(510E6);
    if (status) {
        LoRa.setSpreadingFactor(10);
        LoRa.setSignalBandwidth(250E3);
        LoRa.setSyncWord(0xCC);
        LoRa.enableCrc();
        LoRa.idle();
        return true;
    }
    return false;
}

void LoraNet::sendData(Packet &p) {

    unsigned long start, m;
    start = millis();
    while (LoRa.beginPacket() == 0) {
        m = millis();
        if ((m - start) > WAIT_TRANSMITTING) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::sendData] Wait transmitting failed\n")));
            break;
        }
        yield();
    }

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[LoraNet::sendData] Sender: %i, Sport: %u, Receiver: %i, Rport: %u, Cmd: %i, Data: %ld\n"),
                     p.sender, p.sp, p.receiver, p.rp, p.cmd, p.data));

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
    status = LoRa.endPacket();

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::sendData] Sent. Status: %d\n"), status));
}