#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Button.h>
#include <PIR.h>
#include <Relay.h>
#include <Format.h>
#include <EEPROMVar.h>
#include <NetInterface.h>
#include <DisplayHandler.h>

class LightController : public NetInterface, public DisplayHandler, virtual public HandlerInterface {
    static const uint8_t MAX_PIRS = 1;
    static const uint8_t TYPE_PIR = 0;
    static const uint8_t TYPE_ON = 1;
    static const uint8_t TYPE_OFF = 2;
    static const uint8_t TYPE_AUTO = 3;
public:

    EEPROMVar<uint32_t> timeout;
    EEPROMVar<uint16_t> recallTimeout;
    EEPROMVar<uint16_t> activityLimit;
    EEPROMVar<float> activityRatio;
    EEPROMVar<float> recallRatio;
    EEPROMVar<uint8_t> mode;
    EEPROMVar<uint8_t> init;
    EEPROMVar<uint8_t> energyLvl;

    explicit LightController();

    void addRelay(Relay *r) {
        relay = r;
    }

    void addPir(PIR *pir);

    void addButton(Button *btn);

    auto getRelayState() -> int {
        return relay->isOn() ? RELAY_ON : RELAY_OFF;
    };

    void setState(uint8_t state);

    void setMode(uint8_t m);

    void setEnergyLevel(uint8_t lvl);

    void setTimeout(uint32_t t);

    void setActivityRatio(float ratio);

    void setActivityLimit(uint16_t limit);

    void setRecallRatio(float ratio);

    void setRecallTimeout(uint16_t t);

    auto getOffTime() -> long;

    void sendValues();

    void call(uint8_t type, uint8_t idx) override;

    void tick();

protected:

    PIR *pirs[MAX_PIRS]{};
    Relay *relay{};
    Button *modeButton = nullptr;

    unsigned long offTime;
    unsigned long timeOff;
    uint8_t activity;
    uint8_t pirIdx = 0;

    void resetValues();

    void setRelayState(uint8_t state);
};

#endif //LIGHTCONTROLLER_H
