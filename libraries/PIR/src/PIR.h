#ifndef PIR_H
#define PIR_H

#include <Arduino.h>
#include <DebugLog.h>
#include <Switch.h>

class PIR : public Switch {

public:
    explicit PIR(uint8_t btnPin, uint16_t detectDelay = 30, uint8_t max = 1, bool invt = true) :
            Switch(btnPin, max, invt),
            detectDelay(detectDelay * 1000) {}

    void tick() override;

protected:
    uint16_t detectDelay;
};


#endif //PIR_H
