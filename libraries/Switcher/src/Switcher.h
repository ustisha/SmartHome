#ifndef ARDUINOEXAMPLE_SWITCHER_H
#define ARDUINOEXAMPLE_SWITCHER_H

#include <Arduino.h>

class Switcher {
    static const uint8_t MAX = 3;
    static const uint8_t ANALOG_CONNECTED = 200;
protected:

    struct Callback {
        void (*cb)() = NULL;

        uint16_t press = 0;
    };

    Callback arr[MAX];

    uint8_t pin;
    unsigned long start;

    static int sortByPress(const void *elem1, const void *elem2);

    int getIndex();

public:
    static const uint16_t DEFAULT_PRESS = 50;

    explicit Switcher(uint8_t swPin);

    bool isPressed();

    void addHandler(void (*cb)(), unsigned int pressTime);

    void tick();
};


#endif //ARDUINOEXAMPLE_SWITCHER_H
