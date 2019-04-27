#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
public:
    Relay(uint8_t p, bool invt = false);

    void on();

    void off();

    void change();

    bool isOn();

protected:
    uint8_t pin;
    bool invert;
};

#endif //RELAY_H
