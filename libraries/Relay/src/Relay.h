#ifndef RELAY_H
#define RELAY_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>

class Relay {
public:
    Relay(uint8_t p, bool invt = true);

    void on();

    void off();

    void change();

    bool isOn();

protected:
    uint8_t pin;
    bool invert;
};

#endif //RELAY_H
