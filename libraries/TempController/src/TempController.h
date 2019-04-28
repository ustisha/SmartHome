#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
//#include <TInterface.h>
//#include <NetComponent.h>
#include <Relay.h>

class TempController {
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

    void addNet(RadioInterface *radio, NetComponent *net, uint8_t r, uint16_t rp);

    void setTimeout(uint16_t t);

    void tick(uint16_t sleep);

protected:

    void sendCommand(uint8_t cmd, long data);

    void control();

    TInterface *tiface;
    NetComponent *net;
    RadioInterface *radio;
    RelayControl controls[MAX];
    uint8_t r, rp;
    float downLimit, upLimit;
    uint32_t timeout = DEFAULT_INTERVAL;
    unsigned long sleepTime = 0;
    unsigned long last = millis();
};


#endif //TEMPCONTROLLER_H
