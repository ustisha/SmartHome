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

void TempController::setTimeout(uint16_t t) {
    timeout = uint32_t(t * 1000);
}

void TempController::control() {
    if (this->mode != MODE_AUTO) {
        return;
    }
    for (int i = 0; i < MAX; ++i) {
        if (controls[i].enabled) {
            if (controls[i].heat) {
                if (tiface->get() <= (downLimit - controls[i].rangeOn) && !controls[i].relay->isOn()) {
                    relayOn(i);
                } else if (tiface->get() >= (downLimit - controls[i].rangeOff) && controls[i].relay->isOn()) {
                    relayOff(i);
                }
            } else {
                if (tiface->get() >= (upLimit + controls[i].rangeOn) && !controls[i].relay->isOn()) {
                    relayOn(i);
                } else if (tiface->get() <= (upLimit + controls[i].rangeOff) && controls[i].relay->isOn()) {
                    relayOff(i);
                }
            }
        }
    }
}

void TempController::relayOff(uint8_t i) {
    controls[i].relay->off();
    netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay index: %d OFF\n"), i));
}

void TempController::relayOn(uint8_t i) {
    if (this->mode != MODE_AUTO) {
        return;
    }
    controls[i].relay->on();
    netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay index: %d ON\n"), i));
}

int TempController::getRelayState(uint8_t i) {
    if (i >= MAX || !controls[i].enabled) {
        return RELAY_DISABLED;
    }
    return controls[i].relay->isOn() ? RELAY_ON : RELAY_OFF;
}

void TempController::setRelayState(uint8_t i, uint8_t state) {
    if (i >= MAX || !controls[i].enabled) {
        return;
    }
    setMode(MODE_MANUAL);
    if (state == RELAY_ON) {
        relayOn(i);
    } else if (state == RELAY_OFF) {
        relayOff(i);
    }
}

byte TempController::getMode() {
    return this->mode;
}

void TempController::setMode(uint8_t mode) {
    this->mode = mode;
    netComponent->sendCommandData(radio, r, rp, CMD_MODE);
    if (this->mode == MODE_AUTO) {
        control();
    }
}
