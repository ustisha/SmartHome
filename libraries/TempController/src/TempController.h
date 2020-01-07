#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <TInterface.h>
#include <NetInterface.h>
#include <NetComponent.h>
#include <Relay.h>
#include <Servo.h>
#include <Format.h>
#include <EEPROMex.h>
#include <EEPROMvar.h>

class TempController : public NetInterface {
    struct RelayControl {
        bool enabled = false;
        Relay *relay{};
        uint8_t type = 0;
        float rangeOn = 0;
        float rangeOff = 0;
    };
    struct ServoControl {
        bool enabled = false;
        Servo *servo{};
        uint8_t type = 0;
        int minAngle = 0;
        int maxAngle = 0;
        float ratio = 0;
    };
public:
    // 1 бит - Открывать при понижении значения нижней границы
    // 2 бит - Открывать при превышении значения верхней границы
    // 3 бит - Открывать при понижении значения ниже верхней границы
    // 4 бит - Открывать при превышении значения выше нижней границы
    const static uint8_t TYPE_BELOW_DOWN_LIMIT = B00000001;
    const static uint8_t TYPE_ABOVE_UP_LIMIT = B00000010;
    const static uint8_t TYPE_BELOW_UP_LIMIT = B00000100;
    const static uint8_t TYPE_ABOVE_DOWN_LIMIT = B00001000;

    EEPROMVar<float> downLimit;
    EEPROMVar<float> upLimit;
    EEPROMVar<uint8_t> init;
    EEPROMVar<uint8_t> mode;
    EEPROMVar<uint32_t> timeout;

    TempController(TInterface *tiface, uint8_t rMax, uint8_t sMax, float down, float up);

    void addRelay(Relay *r, uint8_t i, uint8_t type, float rangeOn = 0.1, float rangeOff = 0.0);

    void addServo(Servo *s, uint8_t i, uint8_t type, int minAngle = 0, int maxAngle = 90, float ratio = 0.8);

    void tick(uint16_t sleep = 0);

    auto getRelayState(uint8_t i) -> int;

    void setRelayState(uint8_t i, uint8_t state);

    auto getServoState(uint8_t i) -> int;

    void setServoState(uint8_t i, int angle);

    void setDownLimit(float limit);

    void setUpLimit(float limit);

    void setMode(uint8_t m);

    void setTimeout(uint32_t t);

    void sendValues();

protected:

    void control();

    void relayOn(uint8_t i);

    void relayOff(uint8_t i);

    void servoWrite(uint8_t i, int angle);

    TInterface *tiface;
    RelayControl *relayControl;
    ServoControl *servoControl;
    uint8_t relayMax;
    uint8_t servoMax;
    unsigned long sleepTime = 0;
    unsigned long last;
};


#endif //TEMPCONTROLLER_H
