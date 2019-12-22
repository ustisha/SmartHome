#include "TempController.h"

TempController::TempController(uint8_t rMax, uint8_t sMax, TInterface *tiface, float down, float up) :
        relayControl(new RelayControl[rMax]{}),
        servoControl(new ServoControl[sMax]{}),
        relayMax(rMax),
        servoMax(sMax),
        tiface(tiface),
        downLimit(down),
        upLimit(up) {

    mode = MODE_AUTO;
    last = millis();

    downLimit.restore();
    upLimit.restore();

    if (abs(downLimit - down) > 100) {
        downLimit = down;
        downLimit.save();
    }

    if (abs(upLimit - upLimit) > 100) {
        upLimit = up;
        upLimit.save();
    }

#ifdef SERIAL_DEBUG
    static char dlBuf[8];
    static char ulBuf[8];
    Format::floatVar(dlBuf, downLimit);
    Format::floatVar(ulBuf, upLimit);

    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::TempController] Down limit: %s, Up limit: %s\n"), dlBuf, ulBuf));
#endif
}

void TempController::addRelay(Relay *r, uint8_t i, uint8_t type, float rangeOn, float rangeOff) {
    relayControl[i].enabled = true;
    relayControl[i].relay = r;
    relayControl[i].type = type;
    relayControl[i].rangeOn = rangeOn;
    relayControl[i].rangeOff = rangeOff;

#ifdef SERIAL_DEBUG
    static char onBuf[5];
    static char offBuf[5];
    Format::floatVar(onBuf, rangeOn);
    Format::floatVar(offBuf, rangeOff);

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[TempController::addRelay] Idx: %i, Mode: %d, On: %s, Off: %s\n"),
                     i, (int) type, onBuf, offBuf));
#endif
}

void TempController::addServo(Servo *s, uint8_t i, uint8_t type, int minAngle, int maxAngle, float ratio) {
    servoControl[i].enabled = true;
    servoControl[i].servo = s;
    servoControl[i].type = type;
    servoControl[i].minAngle = minAngle;
    servoControl[i].maxAngle = maxAngle;
    servoControl[i].ratio = ratio;
    servoControl[i].servo->write(minAngle);

#ifdef SERIAL_DEBUG
    static char ratioBuf[5];
    Format::floatVar(ratioBuf, ratio);

    IF_SERIAL_DEBUG(
            printf_P(PSTR(
                    "[TempController::addServo] Idx: %i, Mode: %d, Min angle: %i,  Max angle: %i,Ratio: %s\n"),
                     i, (int) type, minAngle, maxAngle, ratioBuf));
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
//    if (m < last) {
//        last = m;
//    }
}

void TempController::setTimeout(uint16_t t) {
    timeout = (uint32_t) (t * 1000);
}

void TempController::control() {
    if (mode != MODE_AUTO) {
        return;
    }
    for (int i = 0; i < relayMax; ++i) {
        if (relayControl[i].enabled) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::control] Relay idx: %i\n"), i));
            if ((relayControl[i].type & TYPE_BELOW_DOWN_LIMIT &&
                 tiface->get() <= (downLimit - relayControl[i].rangeOn)) ||
                (relayControl[i].type & TYPE_ABOVE_DOWN_LIMIT &&
                 tiface->get() >= (downLimit - relayControl[i].rangeOn))) {
                relayOn(i);
            } else if ((relayControl[i].type & TYPE_BELOW_DOWN_LIMIT &&
                        tiface->get() >= (downLimit - relayControl[i].rangeOff)) ||
                       (relayControl[i].type & TYPE_ABOVE_DOWN_LIMIT &&
                        tiface->get() >= (downLimit - relayControl[i].rangeOff))) {
                relayOff(i);
            }

            if ((relayControl[i].type & TYPE_ABOVE_UP_LIMIT && tiface->get() >= (upLimit + relayControl[i].rangeOn)) ||
                (relayControl[i].type & TYPE_BELOW_UP_LIMIT) && tiface->get() <= (upLimit + relayControl[i].rangeOn)) {
                relayOn(i);
            } else if ((relayControl[i].type & TYPE_ABOVE_UP_LIMIT &&
                        tiface->get() <= (upLimit + relayControl[i].rangeOff)) ||
                       (relayControl[i].type & TYPE_BELOW_UP_LIMIT &&
                        tiface->get() >= (upLimit + relayControl[i].rangeOff))) {
                relayOff(i);
            }
        }
    }
    for (int i = 0; i < servoMax; ++i) {
        if (servoControl[i].enabled) {
            int angle = 0;
            if ((servoControl[i].type & TYPE_BELOW_DOWN_LIMIT && tiface->get() < downLimit) ||
                (servoControl[i].type & TYPE_ABOVE_DOWN_LIMIT && tiface->get() > downLimit)) {
                angle = round(abs(tiface->get() - downLimit) * 10 * servoControl[i].ratio);
            }
            if ((servoControl[i].type & TYPE_ABOVE_UP_LIMIT && tiface->get() > upLimit) ||
                (servoControl[i].type & TYPE_BELOW_UP_LIMIT && tiface->get() < upLimit)) {
                angle = round(abs(tiface->get() - upLimit) * 10 * servoControl[i].ratio);
            }
            servoWrite(i, angle);
        }
    }
}

void TempController::relayOff(uint8_t i) {
    if (!relayControl[i].enabled || !relayControl[i].relay->isOn()) {
        return;
    }
    relayControl[i].relay->off();
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::relayOff] Relay index: %d OFF\n"), i));
    if (networking) {
        netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
    }
}

void TempController::relayOn(uint8_t i) {
    if (!relayControl[i].enabled || relayControl[i].relay->isOn()) {
        return;
    }
    relayControl[i].relay->on();
    IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::relayOn] Relay index: %d ON\n"), i));
    if (networking) {
        netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
    }
}

void TempController::servoWrite(uint8_t i, int angle) {
    if (!servoControl[i].enabled) {
        return;
    }
    if (angle < servoControl[i].minAngle) {
        angle = servoControl[i].minAngle;
    }
    if (angle > servoControl[i].maxAngle) {
        angle = servoControl[i].maxAngle;
    }
    int prevAngle = servoControl[i].servo->read();
    servoControl[i].servo->write(angle);
    if (angle != prevAngle) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[TempController::servoWrite] Servo index: %d Angle: %d\n"), i, angle));
        if (networking) {
            netComponent->sendCommandData(radio, r, rp, CMD_SERVO_00 + i);
        }
    }
}

int TempController::getRelayState(uint8_t i) {
    if (i >= relayMax || !relayControl[i].enabled) {
        return RELAY_DISABLED;
    }
    return relayControl[i].relay->isOn() ? RELAY_ON : RELAY_OFF;
}

void TempController::setRelayState(uint8_t i, uint8_t state) {
    if (i >= relayMax || !relayControl[i].enabled) {
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
    if (i >= servoMax || !servoControl[i].enabled) {
        return SERVO_DISABLED;
    }
    return servoControl[i].servo->read();
}

void TempController::setServoState(uint8_t i, int angle) {
    if (i >= servoMax || !servoControl[i].enabled) {
        return;
    }
    setMode(MODE_MANUAL);
    servoWrite(i, angle);
}

byte TempController::getMode() {
    return mode;
}

void TempController::setMode(uint8_t m) {
    mode = m;
    if (networking) {
        netComponent->sendCommandData(radio, r, rp, CMD_MODE);
    }
    if (mode == MODE_AUTO) {
        control();
    }
}

void TempController::sendValues() {
    if (!networking) {
        return;
    }
    netComponent->sendCommandData(radio, r, rp, CMD_MODE);
    netComponent->sendCommandData(radio, r, rp, CMD_UP_LIMIT);
    netComponent->sendCommandData(radio, r, rp, CMD_DOWN_LIMIT);
    for (int i = 0; i < relayMax; ++i) {
        if (relayControl[i].enabled) {
            netComponent->sendCommandData(radio, r, rp, CMD_RELAY_00 + i);
        }
    }
    for (int i = 0; i < servoMax; ++i) {
        if (servoControl[i].enabled) {
            netComponent->sendCommandData(radio, r, rp, CMD_SERVO_00 + i);
        }
    }
}

void TempController::setDownLimit(long limit) {
    downLimit = (float) limit / 10;
}

void TempController::setUpLimit(long limit) {
    upLimit = (float) limit / 10;
}

float TempController::getDownLimit() {
    return downLimit;
}

float TempController::getUpLimit() {
    return upLimit;
}
