#include "Lamp.h"

Lamp::Lamp(UTFT *d, uint16_t x, uint16_t y, bool time) : baseX(x), baseY(y), timerEnabled(time) {
    display = d;
    currentTime = 0;
    currentMode = MODE_AUTO;
    currentState = false;
    borderColor = 0x655C;
    autoColor = 0x2528;
    manualColor = 0xD1A8;
}

void Lamp::renderContent() {
    display->setColor(borderColor);
    display->drawRoundRect(baseX, baseY, baseX + 32, baseY + 16);
    if (currentState) {
        display->drawBitmap(baseX + 26, baseY + 1, 9, 16, lampOn16px);
    } else {
        display->drawBitmap(baseX + 26, baseY + 1, 9, 16, lampOff16px);
    }
    renderTime();
}

void Lamp::renderState() {
    display->setFont(mykefont2);
    if (currentMode == MODE_AUTO) {
        display->setColor(autoColor);
        display->print(" A ", baseX + 2, baseY + 3);
    } else if (currentMode == MODE_MANUAL) {
        display->setColor(manualColor);
        if (currentState) {
            display->print("ON ", baseX + 2, baseY + 3);
        } else {
            display->print("OFF", baseX + 2, baseY + 3);
        }
    }
}

void Lamp::renderTime() {
    if (currentTime > 0) {
        display->setFont(mykefont2);
        String timeout = "";
        if (currentTime < 10) {
            timeout.concat(" ");
            timeout.concat(currentTime);
            timeout.concat(" ");
        } else if (currentTime < 100) {
            timeout.concat(" ");
            timeout.concat(currentTime);
        } else {
            timeout.concat(currentTime);
        }
        display->print(timeout, baseX + 2, baseY + 3);
    } else {
        renderState();
    }
}
