#include "LightController.h"

LightController::LightController(uint16_t t, float act, float re) : timeout(t * 1000), activityRatio(act),
                                                          activityLimit(2), recallRatio(re),
                                                          recallTimeout(2) {
    timeout.restore();
    activityRatio.restore();
    activityLimit.restore();
    recallRatio.restore();
    recallTimeout.restore();

#ifdef SERIAL_DEBUG
    static char actBuf[8];
    static char reBuf[8];
    Format::floatVar(actBuf, activityRatio);
    Format::floatVar(reBuf, recallRatio);

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[LightRelay] Timeout: %u, Activity ratio: %s; limit: %u,  Recall ratio: %s; timeout: %u\n"),
                     timeout, actBuf, activityLimit, reBuf, recallTimeout));
#endif
}

void LightController::addRelay(Relay *r) {
    relay = r;
}

void LightController::addButton(Button *b) {
    buttons[buttonIdx++] = b;
}

void LightController::call(uint8_t idx) {
    activity++;
    start = millis();
    if (relay->isOn()) {
        if (activity > 1 && activity <= activityLimit) {
            timeOff += lround(timeout * activityRatio);
        }
    } else {
        if ((start - timeOff) < recallTimeout) {
            timeOff = start + lround(timeout * recallRatio);
            IF_SERIAL_DEBUG(PSTR("[LightRelay::call] Recall raise\n"));
        } else {
            timeOff = start + timeout;
        }
    }
    IF_SERIAL_DEBUG(
            printf_P(PSTR("[LightRelay::call] Relay on. Timeout in (sec): %ld\n"),
                     lround((timeOff - start) / 1000)));

    relay->on();
}

void LightController::tick() {
    for (uint8_t b = 0; b < buttonIdx; b++) {
        buttons[b]->tick();
    }
    uint32_t m = millis();
    if (m > (start + timeOff)) {
        IF_SERIAL_DEBUG(PSTR("[LightRelay::tick] Relay off\n"));
        relay->off();
        activity = 0;
    }
}
