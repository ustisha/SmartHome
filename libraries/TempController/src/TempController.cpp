#include "TempController.h"

TempController::TempController(TInterface *tiface, float downLimit, float upLimit) : tiface(tiface),
                                                                                     downLimit(downLimit),
                                                                                     upLimit(upLimit) {
    mode = MODE_AUTO;
    last = millis();

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
    relayControl[i].enabled = true;
    relayControl[i].relay = r;
    relayControl[i].heat = heat;
    relayControl[i].rangeOn = rangeOn;
    relayControl[i].rangeOff = rangeOff;

#ifdef SERIAL_DEBUG
    String on(rangeOn);
    static char onBuf[5];
    on.toCharArray(onBuf, 5);

    String off(rangeOff);
    static char offBuf[5];
    off.toCharArray(offBuf, 5);

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[TempController::addRelay] Idx: %i, Mode: %d, On: %s, Off: %s\n"),
                     i, (int) heat, onBuf, offBuf));
#endif
}

void TempController::addServo(Servo *s, uint8_t i, bool heat, int angle, float ratio) {
    servoControl[i].enabled = true;
    servoControl[i].servo = s;
    servoControl[i].heat = heat;
    servoControl[i].maxAngle = angle;
    servoControl[i].ratio = ratio;
    servoControl[i].servo->write(0);

#ifdef SERIAL_DEBUG
    String rto(ratio);
    static char ratioBuf[5];
    rto.toCharArray(ratioBuf, 5);

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[TempController::addServo] Idx: %i, Mode: %d, Angle: %i, Ratio: %s\n"),
                     i, (int) heat, angle, ratioBuf));
#endif
}

void TempController::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    if (m >= (last + timeout) || last < timeout) {
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
        if (relayControl[i].enabled) {
            if (relayControl[i].heat) {
                if (tiface->get() <= (downLimit - relayControl[i].rangeOn) && !relayControl[i].relay->isOn()) {
                    relayOn(i);
                } else if (tiface->get() >= (downLimit - relayControl[i].rangeOff) && relayControl[i].relay->isOn()) {
                    relayOff(i);
                }
            } else {
                if (tiface->get() >= (upLimit + relayControl[i].rangeOn) && !relayControl[i].relay->isOn()) {
                    relayOn(i);
                } else if (tiface->get() <= (upLimit + relayControl[i].rangeOff) && relayControl[i].relay->isOn()) {
                    relayOff(i);
                }
            }
        }
        if (servoControl[i].enabled) {
            int angle = 0;
            if (servoControl[i].heat) {
                if (tiface->get() < downLimit) {
                    angle = round(abs(tiface->get() - downLimit) * 10 * servoControl[i].ratio);
                }
            } else {
                if (tiface->get() > upLimit) {
                    angle = round(abs(tiface->get() - upLimit) * 10 * servoControl[i].ratio);
                }
            }
            servoWrite(i, angle);
        }
    }
}

void TempController::relayOff(uint8_t i) {
    if (!relayControl[i].enabled) {
        return;
    }
    relayControl[i].relay->off();
    netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::relayOff] Relay index: %d OFF\n"), i));
}

void TempController::relayOn(uint8_t i) {
    if (!relayControl[i].enabled) {
        return;
    }
    relayControl[i].relay->on();
    netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::relayOn] Relay index: %d ON\n"), i));
}

void TempController::servoWrite(uint8_t i, int angle) {
    if (!servoControl[i].enabled) {
        return;
    }
    if (angle < 0) {
        angle = 0;
    }
    if (angle > servoControl[i].maxAngle) {
        angle = servoControl[i].maxAngle;
    }
    int prevAngle = servoControl[i].servo->read();
    servoControl[i].servo->write(angle);
    if (angle != prevAngle) {
        netComponent->sendCommandData(radio, r, rp, CMD_SERVO_00 + i);
        IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::servoWrite] Servo index: %d Angle: %d\n"), i, angle));
    }
}

int TempController::getRelayState(uint8_t i) {
    if (i >= MAX || !relayControl[i].enabled) {
        return RELAY_DISABLED;
    }
    return relayControl[i].relay->isOn() ? RELAY_ON : RELAY_OFF;
}

void TempController::setRelayState(uint8_t i, uint8_t state) {
    if (i >= MAX || !relayControl[i].enabled) {
        return;
    }
    setMode(MODE_MANUAL);
    if (state == RELAY_ON) {
        relayOn(i);
    } else if (state == RELAY_OFF) {
        relayOff(i);
    }
}

int TempController::getServoState(uint8_t i) {
    if (i >= MAX || !servoControl[i].enabled) {
        return SERVO_DISABLED;
    }
    return servoControl[i].servo->read();
}

void TempController::setServoState(uint8_t i, int angle) {
    if (i >= MAX || !servoControl[i].enabled) {
        return;
    }
    setMode(MODE_MANUAL);
    servoWrite(i, angle);
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
