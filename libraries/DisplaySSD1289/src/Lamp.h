#ifndef LAMP_H
#define LAMP_H

#include <UTFT.h>
#include <DebugLog.h>
#include <Net.h>
#include <Format.h>

extern unsigned int lampOff16px[144];
extern unsigned int lampOn16px[144];
extern uint8_t mykefont2[];

class Lamp {

public:
    uint8_t currentMode;
    uint16_t currentTime;
    bool currentState;

    Lamp(UTFT *d, uint16_t x, uint16_t y, bool timer = false);

    void renderContent();

    void renderTime();

    void renderState();

protected:
    UTFT *display;
    uint16_t baseX, baseY;
    bool timerEnabled;
    word borderColor, autoColor, manualColor;
};


#endif //LAMP_H
