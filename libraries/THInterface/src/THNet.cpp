#include "THNet.h"

void THNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    if (cmd == CMD_TEMPERATURE) {
        sendPacket(n, r, rp, cmd, tempHum->get() * 100);
    } else if (cmd == CMD_HUMIDITY) {
        sendPacket(n, r, rp, cmd, tempHum->getHumidity() * 100);
    }
}
