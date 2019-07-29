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

class TempController : public NetInterface {
    const static uint8_t MAX = 2;
    const static uint32_t DEFAULT_INTERVAL = 5 * 60000;
    struct RelayControl {
        bool enabled = false;
        Relay *relay;
        bool heat = true;
        float rangeOn = 0;
        float rangeOff = 0;
    };
    struct ServoControl {
        bool enabled = false;
        Servo *servo;
        bool heat = true;
        int maxAngle = 0;
        float ratio = 0;
    };
public:
    TempController(TInterface *tiface, float downLimit, float upLimit);

    void addRelay(Relay *r, uint8_t i, bool heat, float rangeOn = 0.1, float rangeOff = 0.0);

    void addServo(Servo *s, uint8_t i, bool heat, int angle = 90, float ratio = 0.8);

    void setTimeout(uint16_t t);

    void tick(uint16_t sleep = 0);

    int getRelayState(uint8_t i);

    void setRelayState(uint8_t i, uint8_t state);

    int getServoState(uint8_t i);

    void setServoState(uint8_t i, int angle);

    byte getMode();

    void setMode(uint8_t mode);

protected:

    void control();

    void relayOn(uint8_t i);

    void relayOff(uint8_t i);

    void servoWrite(uint8_t i, int angle);

    TInterface *tiface;
    RelayControl relayControl[MAX];
    ServoControl servoControl[MAX];
    float downLimit, upLimit;
    uint8_t mode = MODE_AUTO;
    uint32_t timeout = DEFAULT_INTERVAL;
    unsigned long sleepTime = 0;
    unsigned long last;
};


#endif //TEMPCONTROLLER_H
