#include "LoraNetReceive.h"

void LoraNetReceive::sendData(Packet &p) {
    LoraNet::sendData(p);
    LoRa.receive();
    delayMicroseconds(100);
    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNet::sendData] Receive mode\n")));
}
