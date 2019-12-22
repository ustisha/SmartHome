#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <HandlerInterface.h>

class Button {
    static const uint8_t MAX = 2;
    static const uint8_t ANALOG_CONNECTED = 200;
protected:

    struct Callback {
        HandlerInterface *handlerInterface = NULL;
        uint16_t press = 0;
        bool enabled = false;
    };

    Callback arr[MAX];

    uint8_t pin;
    unsigned long start;

    static int sortByPress(const void *elem1, const void *elem2);

    uint8_t getIndex();

public:
    static const uint16_t DEFAULT_PRESS = 50;

    explicit Button(uint8_t btnPin);

    bool isPressed();

    uint8_t addHandler(HandlerInterface *handlerInterface, uint16_t pressTime = DEFAULT_PRESS);

    void tick();
};


#endif //BUTTON_H
