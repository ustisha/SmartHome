#ifndef TEMPCONTROLLERNET_H
#define TEMPCONTROLLERNET_H

#include <Arduino.h>
#include <DebugLog.h>
#include <NetComponent.h>
#include <TempController.h>

class TempControllerNet : public NetComponent {

public:
    TempControllerNet(SmartNet *n, uint8_t sp, uint8_t max, TempController *ctrl) : NetComponent(n, sp, max) {
        tempCtrl = ctrl;
    }

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

    void receiveCommandData(uint8_t cmd, long data) override;

protected:
    TempController *tempCtrl;
};

#endif //TEMPCONTROLLERNET_H
