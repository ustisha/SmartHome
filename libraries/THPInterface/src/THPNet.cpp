#include "THPNet.h"

void THPNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    if (cmd == CMD_TEMPERATURE) {
        sendPacket(n, r, rp, cmd, tempHumPressure->get() * 100);
    } else if (cmd == CMD_HUMIDITY) {
        sendPacket(n, r, rp, cmd, tempHumPressure->getHumidity() * 100);
    } else if (cmd == CMD_PRESSURE) {
        sendPacket(n, r, rp, cmd, tempHumPressure->getPressure() * 100);
    }
}
