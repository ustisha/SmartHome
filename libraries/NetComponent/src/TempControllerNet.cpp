#include "TempControllerNet.h"

TempControllerNet::TempControllerNet(SmartNet *n, uint8_t sp, uint8_t max, TempController *tCtrl) :
        NetComponent(n, sp, max) {
    tempCtrl = tCtrl;
}

void TempControllerNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    Packet p{};
    p.setSender(net->sender);
    p.setSenderPort(sport);
    p.setReceiver(r);
    p.setReceiverPort(rp);
    p.setCommand(cmd);

    if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        p.setData(tempCtrl->getRelayState(cmd - CMD_RELAY_00));
    } else if (cmd >= CMD_SERVO_00 && cmd <= CMD_SERVO_09) {
        p.setData(tempCtrl->getServoState(cmd - CMD_SERVO_00));
    } else if (cmd == CMD_MODE) {
        p.setData(tempCtrl->getMode());
    } else if (cmd == CMD_DOWN_LIMIT) {
        p.setData((long) (tempCtrl->getDownLimit() * 10));
    } else if (cmd == CMD_UP_LIMIT) {
        p.setData((long) (tempCtrl->getUpLimit() * 10));
    }

    n->sendData(&p);
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
        tempCtrl->setDownLimit(data);
    } else if (cmd == CMD_UP_LIMIT) {
        tempCtrl->setUpLimit(data);
    }
}
