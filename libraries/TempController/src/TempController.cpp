#include "TempController.h"

TempController::TempController(TInterface *tiface, float downLimit, float upLimit) : tiface(tiface),
                                                                                     downLimit(downLimit),
                                                                                     upLimit(upLimit) {

#ifdef SERIAL_DEBUG
    String dl(downLimit);
    static char dlBuf[8];
    dl.toCharArray(dlBuf, 8);

    String ul(upLimit);
    static char ulBuf[8];
    ul.toCharArray(ulBuf, 8);

    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::TempController] Down limit: %s, Up limit: %s\n"), dlBuf, ulBuf));
#endif
}

void TempController::addRelay(Relay *r, uint8_t i, bool heat, float rangeOn, float rangeOff) {
    controls[i].enabled = true;
    controls[i].relay = r;
    controls[i].heat = heat;
    controls[i].rangeOn = rangeOn;
    controls[i].rangeOff = rangeOff;

#ifdef SERIAL_DEBUG
    String on(rangeOn);
    static char onBuf[8];
    on.toCharArray(onBuf, 8);

    String off(rangeOff);
    static char offBuf[8];
    off.toCharArray(offBuf, 8);

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[TempController::addRelay] Idx: %i, Mode: %d, On: %s, Off: %s\n"),
                     i, (int) heat, onBuf, offBuf));
#endif
}

void TempController::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    if (m >= (last + timeout)) {
        last += timeout;
        control();
    }
    if (m < last) {
        last = m;
    }
}

void TempController::addNet(RadioInterface *radio, NetComponent *net, uint8_t r) {
    this->radio = radio;
    this->net = net;
    this->r = r;
}

void TempController::sendCommand(uint16_t rp, uint8_t cmd, long data) {
//    net->sendCommandData(this->radio, )
}


void TempController::setTimeout(uint16_t t) {
    timeout = uint32_t(t * 1000);
}

void TempController::control() {
    for (int i = 0; i < MAX; ++i) {
        if (controls[i].enabled) {
            if (controls[i].heat) {
                if (tiface->get() <= (downLimit - controls[i].rangeOn) && !controls[i].relay->isOn()) {
                    controls[i].relay->on();
                    sendCommand(3000 + i, CMD_ON, );
                    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay index: %d ON\n"), i));
                } else if (tiface->get() >= (downLimit - controls[i].rangeOff) && controls[i].relay->isOn()) {
                    controls[i].relay->off();
                    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay index: %d OFF\n"), i));
                }
            } else {
                if (tiface->get() >= (upLimit + controls[i].rangeOn) && !controls[i].relay->isOn()) {
                    controls[i].relay->on();
                    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay index: %d ON\n"), i));
                } else if (tiface->get() <= (upLimit + controls[i].rangeOff) && controls[i].relay->isOn()) {
                    controls[i].relay->off();
                    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay index: %d OFF\n"), i));
                }
            }
        }
    }
}
