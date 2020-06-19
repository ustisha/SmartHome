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
        uint8_t type = 0;
        uint16_t press = 0;
        uint8_t idx = 0;
    };

    Callback *arr;

    uint8_t i;
    uint8_t maxArr;
    uint8_t pin;
    unsigned long start;
    bool invert;

    static auto sortByPress(const void *elem1, const void *elem2) -> int;

public:
    static const uint16_t PRESSTIME_DEFAULT = 50;
    static const uint16_t PRESSTIME_1SEC = 1000;
    static const uint16_t PRESSTIME_2SEC = 2000;
    static const uint16_t PRESSTIME_4SEC = 4000;

    explicit Button(uint8_t btnPin, uint8_t max = 1, bool invt = true);

    auto isPressed() -> bool;

    auto addHandler(HandlerInterface *handlerInterface, uint8_t t = 0, uint16_t pressTime = PRESSTIME_DEFAULT, uint8_t i = 0) -> int8_t;

    virtual void tick();
};


#endif //BUTTON_H
