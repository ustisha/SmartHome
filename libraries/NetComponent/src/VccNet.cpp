#include "VccNet.h"

void VccNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    sendPacket(n, r, rp, cmd, round(vcc->Read_Volts() * 100));
}
