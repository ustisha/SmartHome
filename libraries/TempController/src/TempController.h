#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <TInterface.h>
#include <NetInterface.h>
#include <NetComponent.h>
#include <Relay.h>

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
public:
    TempController(TInterface *tiface, float downLimit, float upLimit);

    void addRelay(Relay *r, uint8_t i, bool heat, float rangeOn = 0.1, float rangeOff = 0.0);

    void setTimeout(uint16_t t);

    void tick(uint16_t sleep);

    int getRelayState(uint8_t i);

    void setRelayState(uint8_t i, uint8_t state);

    byte getMode();

    void setMode(uint8_t mode);

protected:

    void control();

    void relayOn(uint8_t i);

    void relayOff(uint8_t i);

    TInterface *tiface;
    RelayControl controls[MAX];
    float downLimit, upLimit;
    uint8_t mode;
    uint32_t timeout = DEFAULT_INTERVAL;
    unsigned long sleepTime = 0;
    unsigned long last = millis();
};


#endif //TEMPCONTROLLER_H
