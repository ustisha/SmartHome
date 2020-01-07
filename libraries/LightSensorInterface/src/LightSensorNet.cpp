#include "LightSensorNet.h"

void LightSensorNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    lightSensor->read();
    sendPacket(n, r, rp, cmd, round(lightSensor->get() * 100));
}
