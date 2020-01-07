#include "ValueIntNet.h"

void ValueIntNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    sendPacket(n, r, rp, cmd, valueInt->get());
}
