#include <src/Format.h>
#include "LoraNet.h"

LoraNet::LoraNet(SmartNet *net, uint8_t ss, uint8_t reset, uint8_t dio0) : RadioInterface(net) {
    //    LoRa.setSPIFrequency(4E6);
    LoRa.setPins(ss, reset, dio0);
    int status;
    status = LoRa.begin(460E6);
    if (status) {
        LoRa.setSignalBandwidth(125E3);
        LoRa.setSyncWord(0xAA);
        LoRa.setTxPower(18);
        LoRa.enableCrc();
        LoRa.idle();
        delay(10);
    }

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet] Initialize status: %d\n"), status));
}

void LoraNet::sendData(Packet *p) {

    unsigned long start;
    unsigned long m;
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
            printf_P(PSTR("[LoraNet::sendData] Sender: %u, Sport: %u, Receiver: %u, Rport: %u, Cmd: %u, Data: %ld\n"),
                     p->getSender(), p->getSenderPort(), p->getReceiver(), p->getReceiverPort(), p->getCommand(),
                     p->getData()));

    for (int i = 0; i < Packet::PACKET_SIZE; i++) {
        LoRa.write(p->b[i]);
    }
    int status = LoRa.endPacket();
    delay(10);

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::sendData] Sent. Status: %d\n"), status));
}

void LoraNet::onReceiveFunc(void (*callback)(int)) {
    LoRa.onReceive(callback);
    LoRa.receive();
}

void LoraNet::receiveProcess(SmartNet *net, int packetSize, uint8_t receiver) {

#ifdef SERIAL_DEBUG
    char snrBuf[8];
    Format::floatVar(snrBuf, LoRa.packetSnr());

    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::receiveProcess] LoRa packet. Size: %d, RSSI: %d, SNR: %s\n"),
                             packetSize, LoRa.packetRssi(), snrBuf));
#endif

    if (packetSize != Packet::PACKET_SIZE) {
        while (LoRa.read() != -1) {
            yield();
        }
        return;
    }

    Packet *p;
    p = new Packet();
    for (int i = 0; i < Packet::PACKET_SIZE; i++) {
        p->b[i] = LoRa.read();
    }

    if (p->getReceiver() != receiver && p->getReceiver() != BROADCAST) {
        return;
    }

    net->commandReceived(p);
}
