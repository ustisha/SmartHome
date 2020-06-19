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

void LightControllerNet::receiveCommandData(uint8_t cmd, long data) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[LightControllerNet::receiveCommandData] Cmd: %i, Data: %ld\n"), cmd, data));

    if (cmd == CMD_INFO) {
        if (data == CMD_GET_VALUES) {
            lightCtrl->sendValues();
        }
    } else if (cmd >= CMD_RELAY_00 && cmd <= CMD_RELAY_09) {
        lightCtrl->setState(data);
    } else if (cmd == CMD_MODE) {
        lightCtrl->setMode(data);
    } else if (cmd == CMD_TIMEOUT) {
        lightCtrl->setTimeout(data);
    } else if (cmd == CMD_ACTIVITY_RATIO) {
        lightCtrl->setActivityRatio(float(data) / 100);
    } else if (cmd == CMD_ACTIVITY_LIMIT) {
        lightCtrl->setActivityLimit(data);
    } else if (cmd == CMD_RECALL_RATIO) {
        lightCtrl->setRecallRatio(float(data) / 100);
    } else if (cmd == CMD_RECALL_TIMEOUT) {
        lightCtrl->setRecallTimeout(data);
    } else if (cmd == CMD_CALL) {
        lightCtrl->call(data, 0);
    } else if (cmd == CMD_ENERGY_LEVEL) {
        lightCtrl->setEnergyLevel(data);
    }
}
