#ifndef BUTTON_H
#define BUTTON_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <HandlerInterface.h>

class Button {
    static const uint8_t ANALOG_CONNECTED = 200;
protected:

    struct Callback {
        HandlerInterface *handlerInterface = nullptr;
        uint16_t press = 0;
    };

    Callback *arr;

    uint8_t i;
    uint8_t maxArr;
    uint8_t pin;
    unsigned long start;
    bool invert;

    static auto sortByPress(const void *elem1, const void *elem2) -> int;

public:
    static const uint16_t DEFAULT_PRESS = 50;

    explicit Button(uint8_t btnPin, uint8_t max = 1, bool invt = true);

    auto isPressed() -> bool;

    auto addHandler(HandlerInterface *handlerInterface, uint16_t pressTime = DEFAULT_PRESS) -> int8_t;

    virtual void tick();
};


#endif //BUTTON_H
