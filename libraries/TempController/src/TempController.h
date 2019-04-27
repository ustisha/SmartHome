#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <TInterface.h>
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
    TempController(TInterface *tiface, float required);

    void addRelay(Relay *r, uint8_t i, bool heat, float rangeOn = 0.1, float rangeOff = 0.2);

    void setTimeout(uint32_t t);

    void control();

    void tick(uint16_t sleep);

protected:
    TInterface *tiface;
    RelayControl controls[MAX];
    float required;
    uint32_t timeout = DEFAULT_INTERVAL;
    unsigned long sleepTime = 0;
    unsigned long last = millis();
};


#endif //TEMPCONTROLLER_H
