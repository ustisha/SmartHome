#ifndef TEMP_H
#define TEMP_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <DisplayHandler.h>

class TInterface: public DisplayHandler {
    const uint32_t DEFAULT_POLL_INTERVAL = 30000;
public:
    TInterface() = default;

    float get() {
        return temp;
    }

    int getStatus() {
        return status;
    }

    void setPollInterval(uint16_t tout) {
        timeout = tout * 1000;
    }

    void tick(uint16_t sleep = 0);

protected:
    float temp = -1;
    int status = -1;
    unsigned long sleepTime = 0;
    unsigned long last = millis();
    uint32_t timeout = DEFAULT_POLL_INTERVAL;

    virtual void read() = 0;
};

#endif //TEMP_H
