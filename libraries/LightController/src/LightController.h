#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Button.h>
#include <Relay.h>
#include <HandlerInterface.h>
#include <Format.h>
#include <EEPROMVar.h>

class LightController : public HandlerInterface {
    static const uint8_t MAX_BUTTONS = 2;

public:

    // Таймаут выключения в секундах.
    static const uint16_t DEFAULT_TIMEOUT = 60;

    bool enabled = false;
    uint8_t buttonIdx = 0;
    Button *buttons[MAX_BUTTONS];
    Relay *relay{};

    explicit LightController(uint16_t t = DEFAULT_TIMEOUT, float act = 2, float re = 2);

    void addRelay(Relay *r);

    void addButton(Button *b);

    void call(uint8_t idx) override;

    void tick();

protected:

    unsigned long start = 0;
    unsigned long timeOff = 0;
    uint8_t activity = 0;
    EEPROMVar<uint16_t> timeout;
    EEPROMVar<uint16_t> recallTimeout;
    EEPROMVar<uint16_t> activityLimit;
    EEPROMVar<float> activityRatio;
    EEPROMVar<float> recallRatio;
};

#endif //LIGHTCONTROLLER_H
