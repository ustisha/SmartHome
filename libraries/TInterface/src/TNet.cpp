#include "TNet.h"

void TNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    sendPacket(n, r, rp, cmd, round(temperature->get() * 100));
}