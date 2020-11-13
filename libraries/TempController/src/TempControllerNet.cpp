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

void TempControllerNet::receiveCommandData(Packet *p) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempControllerNet::receiveCommandData] Cmd: %i, Data: %ld\n"), p->getCommand(), p->getData()));

    if (p->getCommand() == CMD_INFO) {
        if (p->getData() == CMD_GET_VALUES) {
            tempCtrl->sendValues();
        }
    } else if (p->getCommand() >= CMD_RELAY_00 && p->getCommand() <= CMD_RELAY_09) {
        tempCtrl->setRelayState(p->getCommand() - CMD_RELAY_00, p->getData());
    } else if (p->getCommand() >= CMD_SERVO_00 && p->getCommand() <= CMD_SERVO_09) {
        tempCtrl->setServoState(p->getCommand() - CMD_SERVO_00, p->getData());
    } else if (p->getCommand() == CMD_MODE) {
        tempCtrl->setMode(p->getData());
    } else if (p->getCommand() == CMD_DOWN_LIMIT) {
        tempCtrl->setDownLimit(float(p->getData()) / 100);
    } else if (p->getCommand() == CMD_UP_LIMIT) {
        tempCtrl->setUpLimit(float(p->getData()) / 100);
    } else if (p->getCommand() == CMD_TIMEOUT) {
        tempCtrl->setTimeout(p->getData());
    }
}
