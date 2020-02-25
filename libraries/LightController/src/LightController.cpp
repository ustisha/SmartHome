#include "LightController.h"

LightController::LightController() : timeout(60 * 1000),
                                     activityRatio(0.8),
                                     activityLimit(2),
                                     recallRatio(2.2),
                                     recallTimeout(2 * 1000),
                                     mode(MODE_AUTO),
                                     init(0),
                                     offTime(0),
                                     timeOff(0),
                                     activity(0) {

    init.restore();
    if (init != 1) {
        mode.save();
        timeout.save();
        activityRatio.save();
        activityLimit.save();
        recallRatio.save();
        recallTimeout.save();

        init = 1;
        init.save();
        IF_SERIAL_DEBUG(printf_P(PSTR("[LightController] Save default\n")));
    } else {
        mode.restore();
        timeout.restore();
        activityRatio.restore();
        activityLimit.restore();
        recallRatio.restore();
        recallTimeout.restore();

        IF_SERIAL_DEBUG(printf_P(PSTR("[LightController] Restore values\n")));
    }

#ifdef SERIAL_DEBUG

    char actBuf[8];
    char reBuf[8];
    Format::floatVar(actBuf, activityRatio);
    Format::floatVar(reBuf, recallRatio);

    IF_SERIAL_DEBUG(
            printf_P(
                    PSTR("[LightController] Mode: %u, Timeout: %u, Act ratio: %s, Act limit: %u, Re ratio: %s, Re timeout: %u\n"),
                    (uint8_t) mode, (uint16_t) timeout, actBuf, (uint16_t) activityLimit, reBuf,
                    (uint16_t) recallTimeout));
#endif
}

void LightController::addPir(PIR *pir) {
    pir->addHandler(this, TYPE_PIR);
    pirs[pirIdx++] = pir;
    IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::addPir] PIR sensor added. Idx: %u\n"), pirIdx));
}

void LightController::addButton(Button *btn) {
    btn->addHandler(this, TYPE_AUTO);
    btn->addHandler(this, TYPE_ON, Button::PRESSTIME_2SEC);
    btn->addHandler(this, TYPE_OFF, Button::PRESSTIME_4SEC);
    modeButton = btn;
    IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::addButton] Control button added.\n")));
}

void LightController::call(uint8_t type, uint8_t idx) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::call] Type: %u, Index: %u \n"), type, idx));
    if (type == TYPE_AUTO) {
        setMode(MODE_AUTO);
        this->call(TYPE_PIR, 0);
    } else if (type == TYPE_ON) {
        setMode(MODE_MANUAL);
        setRelayState(RELAY_ON);
    } else if (type == TYPE_OFF) {
        setMode(MODE_MANUAL);
        setRelayState(RELAY_OFF);
    } else if (type == TYPE_PIR && mode == MODE_AUTO) {
        uint32_t newTime = 0;
        newTime = millis() + timeout;
        if (offTime != 0 && (millis() - offTime) < recallTimeout) {
            newTime += lround(timeout * recallRatio);
            IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::call] Recall raise\n")));
        } else if (activity >= 1) {
            if (activity > activityLimit) {
                activity = activityLimit;
            }
            newTime += lround(timeout * activityRatio * activity);
            IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::call] Activity increase\n")));
        }

        // Use max time already set or new calculated.
        timeOff = max(timeOff, newTime);

        activity++;
        setRelayState(RELAY_ON);
    }
}

void LightController::setMode(uint8_t m) {
    mode = m;
    mode.save();
    sendCommand(CMD_MODE);
    if (mode == MODE_AUTO) {
        setRelayState(RELAY_OFF);
        activity = 0;
        timeOff = 0;
    }
}

void LightController::setTimeout(uint32_t t) {
    timeout = t;
    timeout.save();
    sendCommand(CMD_TIMEOUT);
}

void LightController::setActivityRatio(float ratio) {
    activityRatio = ratio;
    activityRatio.save();
    sendCommand(CMD_ACTIVITY_RATIO);
}

void LightController::setActivityLimit(uint16_t limit) {
    activityLimit = limit;
    activityLimit.save();
    sendCommand(CMD_ACTIVITY_LIMIT);
}

void LightController::setRecallRatio(float ratio) {
    recallRatio = ratio;
    recallRatio.save();
    sendCommand(CMD_RECALL_RATIO);
}

void LightController::setRecallTimeout(uint16_t t) {
    recallTimeout = t * 1000;
    recallTimeout.save();
    sendCommand(CMD_RECALL_TIMEOUT);
}

void LightController::setRelayState(uint8_t state) {
    if (state == RELAY_ON) {
        relay->on();
    } else if (state == RELAY_OFF) {
        relay->off();
    }
    sendCommand(CMD_RELAY_00);
}

void LightController::setState(uint8_t state) {
    setMode(MODE_MANUAL);
    setRelayState(state);
}

auto LightController::getOffTime() -> long {
    return lround((timeOff - millis()) / 1000);
}

void LightController::tick() {
    if (modeButton != nullptr) {
        modeButton->tick();
    }

    if (mode == MODE_MANUAL) {
        return;
    }
    for (uint8_t b = 0; b < pirIdx; b++) {
        pirs[b]->tick();
    }
    unsigned long m = millis();

    if (timeOff != 0 && (m % 1000) == 0) {
        sendCommand(CMD_TIME_LEFT);
        IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::tick] Left: %ld\n"), getOffTime()));
    }

    if (timeOff != 0 && m > timeOff) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[LightController::tick] Reset values\n")));
        setRelayState(RELAY_OFF);
        activity = 0;
        timeOff = 0;
        offTime = m;
    }
}

void LightController::sendValues() {
    sendCommand(CMD_MODE);
    sendCommand(CMD_RELAY_00);
    sendCommand(CMD_TIMEOUT);
    sendCommand(CMD_ACTIVITY_RATIO);
    sendCommand(CMD_ACTIVITY_LIMIT);
    sendCommand(CMD_RECALL_RATIO);
    sendCommand(CMD_RECALL_TIMEOUT);
}
