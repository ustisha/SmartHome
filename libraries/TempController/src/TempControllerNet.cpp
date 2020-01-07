#include "TempControllerNet.h"

void TempControllerNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        sendPacket(n, r, rp, cmd, tempCtrl->getRelayState(cmd - CMD_RELAY_00));
    } else if (cmd >= CMD_SERVO_00 && cmd <= CMD_SERVO_09) {
        sendPacket(n, r, rp, cmd, tempCtrl->getServoState(cmd - CMD_SERVO_00));
    } else if (cmd == CMD_MODE) {
        sendPacket(n, r, rp, cmd, tempCtrl->mode);
    } else if (cmd == CMD_DOWN_LIMIT) {
        sendPacket(n, r, rp, cmd, lround(tempCtrl->downLimit * 100));
    } else if (cmd == CMD_UP_LIMIT) {
        sendPacket(n, r, rp, cmd, lround(tempCtrl->upLimit * 100));
    } else if (cmd == CMD_TIMEOUT) {
        sendPacket(n, r, rp, cmd, tempCtrl->timeout);
    }
}

void TempControllerNet::receiveCommandData(uint8_t cmd, long data) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempControllerNet::receiveCommandData] Cmd: %i, Data: %ld\n"), cmd, data));

    if (cmd == CMD_INFO) {
        if (data == CMD_GET_VALUES) {
            tempCtrl->sendValues();
        }
    } else if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        tempCtrl->setRelayState(cmd - CMD_RELAY_00, data);
    } else if (cmd >= CMD_SERVO_00 && cmd <= CMD_SERVO_09) {
        tempCtrl->setServoState(cmd - CMD_SERVO_00, data);
    } else if (cmd == CMD_MODE) {
        tempCtrl->setMode(data);
    } else if (cmd == CMD_DOWN_LIMIT) {
        tempCtrl->setDownLimit(float(data) / 100);
    } else if (cmd == CMD_UP_LIMIT) {
        tempCtrl->setUpLimit(float(data) / 100);
    } else if (cmd == CMD_TIMEOUT) {
        tempCtrl->setTimeout(data);
    }
}
