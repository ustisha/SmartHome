#include "SmartNet.h"

SmartNet::SmartNet(uint16_t s) {
    sender = s;
}

void SmartNet::addRadioChannel(Radio &r, byte idx) {
    channels[idx] = &r;
    LOG("[SmartNet] Radio Channel added. Idx: ");
    LOGVLN(idx);
}

void SmartNet::tick() {
    for (int i = 0; i < 2; ++i) {
        // Прием данных
    }
}

int SmartNet::getIndex() {
    uint8_t i = 0;
    do {
        if (components[i] == NULL) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

void SmartNet::sendPacket(Packet p) {
    LOGLN("[SmartNet] Send packet");
    LOG(" - Sender: ");
    LOGVLN(p.sender.i);
    LOG(" - Sender port: ");
    LOGVLN(p.sp.i);
    LOG(" - Receiver: ");
    LOGVLN(p.receiver.i);
    LOG(" - Receiver port: ");
    LOGVLN(p.rp.i);
    LOG(" - Command: ");
    LOGVLN(p.cmd.i);
    LOG(" - Data: ");
    LOGVLN(p.data.i);

    // Пока используем только один канал.
    channels[0]->sendData(p);
    LOGLN("[SmartNet] Send done");
}

uint16_t SmartNet::getSender() {
    return sender;
}

