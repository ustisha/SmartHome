#include "LoraNetReceive.h"

void LoraNetReceive::sendData(Packet *p) {
    LoraNet::sendData(p);
    LoRa.receive();
    IF_SERIAL_DEBUG(printf_P(PSTR("[LoraNetReceive::sendData] Receive mode\n")));
}
