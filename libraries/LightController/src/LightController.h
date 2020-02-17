#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Switch.h>
#include <Relay.h>
#include <Format.h>
#include <EEPROMVar.h>
#include <NetInterface.h>

class LightController : public NetInterface, virtual public HandlerInterface  {
    static const uint8_t MAX_SWITCHERS = 2;

public:

    Switch *switchers[MAX_SWITCHERS]{};
    Relay *relay{};

    EEPROMVar<uint16_t> timeout;
    EEPROMVar<uint16_t> recallTimeout;
    EEPROMVar<uint16_t> activityLimit;
    EEPROMVar<float> activityRatio;
    EEPROMVar<float> recallRatio;
    EEPROMVar<uint8_t> mode;
    EEPROMVar<uint8_t> init;

    explicit LightController();

    void addRelay(Relay *r) {
        relay = r;
    }

    void addSwitch(Switch *s);

    auto getRelayState() -> int {
        return relay->isOn() ? RELAY_ON : RELAY_OFF;
    };

    void setState(uint8_t state);

    void setMode(uint8_t m);

    void setTimeout(uint32_t t);

    void setActivityRatio(float ratio);

    void setActivityLimit(uint16_t limit);

    void setRecallRatio(float ratio);

    void setRecallTimeout(uint16_t t);

    auto getOffTime() -> long;

    void sendValues();

    void call(uint8_t idx) override;

    void tick();

protected:

    unsigned long offTime;
    unsigned long timeOff;
    uint8_t activity;
    uint8_t switchIdx = 0;

    void setRelayState(uint8_t state);
};

#endif //LIGHTCONTROLLER_H
