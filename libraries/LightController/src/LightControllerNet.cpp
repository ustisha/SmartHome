#include "LightControllerNet.h"

void LightControllerNet::sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) {
    if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        sendPacket(n, r, rp, cmd, lightCtrl->getRelayState());
    } else if (cmd == CMD_MODE) {
        sendPacket(n, r, rp, cmd, lightCtrl->mode);
    } else if (cmd == CMD_TIMEOUT) {
        sendPacket(n, r, rp, cmd, lightCtrl->timeout);
    } else if (cmd == CMD_ACTIVITY_RATIO) {
        sendPacket(n, r, rp, cmd, lround(lightCtrl->activityRatio * 100));
    } else if (cmd == CMD_ACTIVITY_LIMIT) {
        sendPacket(n, r, rp, cmd, lightCtrl->activityLimit);
    } else if (cmd == CMD_RECALL_RATIO) {
        sendPacket(n, r, rp, cmd, lround(lightCtrl->recallRatio * 100));
    } else if (cmd == CMD_RECALL_TIMEOUT) {
        sendPacket(n, r, rp, cmd, lightCtrl->recallTimeout);
    } else if (cmd == CMD_TIME_LEFT) {
        sendPacket(n, r, rp, cmd, lightCtrl->getOffTime());
    } else if (cmd == CMD_ENERGY_LEVEL) {
        sendPacket(n, r, rp, cmd, lightCtrl->energyLvl);
    }
}

void LightControllerNet::receiveCommandData(Packet *p) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[LightControllerNet::receiveCommandData] Cmd: %i, Data: %ld\n"), p->getCommand(), p->getData()));

    if (p->getCommand() == CMD_INFO) {
        if (p->getData() == CMD_GET_VALUES) {
            lightCtrl->sendValues();
        }
    } else if (p->getCommand() >= CMD_RELAY_00 && p->getCommand() <= CMD_RELAY_09) {
        lightCtrl->setState(p->getData());
    } else if (p->getCommand() == CMD_MODE) {
        lightCtrl->setMode(p->getData());
    } else if (p->getCommand() == CMD_TIMEOUT) {
        lightCtrl->setTimeout(p->getData());
    } else if (p->getCommand() == CMD_ACTIVITY_RATIO) {
        lightCtrl->setActivityRatio(float(p->getData()) / 100);
    } else if (p->getCommand() == CMD_ACTIVITY_LIMIT) {
        lightCtrl->setActivityLimit(p->getData());
    } else if (p->getCommand() == CMD_RECALL_RATIO) {
        lightCtrl->setRecallRatio(float(p->getData()) / 100);
    } else if (p->getCommand() == CMD_RECALL_TIMEOUT) {
        lightCtrl->setRecallTimeout(p->getData());
    } else if (p->getCommand() == CMD_CALL) {
        lightCtrl->call(p->getData(), 0);
    } else if (p->getCommand() == CMD_ENERGY_LEVEL) {
        lightCtrl->setEnergyLevel(p->getData());
    }
}
