#include "TempController.h"

TempController::TempController(TInterface *tiface, float required) : tiface(tiface), required(required) {

}

void TempController::addRelay(Relay *r, uint8_t i, bool heat, float rangeOn, float rangeOff) {
    controls[i].enabled = true;
    controls[i].relay = r;
    controls[i].heat = heat;
    controls[i].rangeOn = rangeOn;
    controls[i].rangeOff = rangeOff;
}

void TempController::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    if (m >= (last + timeout)) {
        last += timeout;
        IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::tick] control\n")));
        control();
    }
}

void TempController::setTimeout(uint32_t t) {
    timeout = t;
}

void TempController::control() {
    for (auto & control : controls) {
        if (control.enabled) {
            // 0.1 0.2
            if (control.heat) {
                if (tiface->get() <= (required - control.rangeOn) && !control.relay->isOn()) {
                    control.relay->on();
                 } else if (abs(tiface->get() - required) > control.rangeOff && control.relay->isOn()) {
                    control.relay->off();
                }
            }
        }
    }
}
