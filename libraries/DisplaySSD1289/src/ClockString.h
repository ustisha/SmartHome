#ifndef CLOCKSTRING_H
#define CLOCKSTRING_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Clock.h>
#include <UTFT.h>
#include <Format.h>

extern uint8_t Retro8x16[];

class ClockString : public Clock {

public:
    ClockString(UTFT *d, uint16_t x, uint16_t y);

    void renderContent();

protected:

    UTFT *display;
    word bgColor, textColor;
    tmElements_t savedTime{0};
    uint8_t offset, currentX;
    uint16_t baseX, baseY;

    bool renderDigitValue(uint8_t curr, uint8_t saved);
};


#endif //CLOCKSTRING_H
