#ifndef LIGHTCONTROLLERNET_H
#define LIGHTCONTROLLERNET_H

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <LightController.h>
#include <Net.h>


class LightControllerNet : public NetComponent {

public:
    LightControllerNet(SmartNet *n, uint8_t sp, uint8_t max, LightController *ctrl) : NetComponent(n, sp, max) {
        lightCtrl = ctrl;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

    void receiveCommandData(uint8_t cmd, long data) override;

protected:
    LightController *lightCtrl;
};


#endif //LIGHTCONTROLLERNET_H
