#include "ClockString.h"

ClockString::ClockString(UTFT *d, uint16_t x, uint16_t y) : display(d), baseX(x), baseY(y) {
    bgColor = 0x859A;
    textColor = 0x2124;
    offset = 8;

    savedTime.Year = 254;
    savedTime.Month = 254;
    savedTime.Day = 254;
    savedTime.Hour = 254;
    savedTime.Minute = 254;
    savedTime.Second = 254;

    display->setColor(textColor);
    display->setBackColor(bgColor);
    display->setFont(Retro8x16);
    display->print("/", baseX + (offset * 4), baseY);
    display->print("/", baseX + (offset * 7), baseY);
    display->print(":", baseX + (offset * 14), baseY);
    display->print(":", baseX + (offset * 17), baseY);
}

bool ClockString::renderDigitValue(uint8_t curr, uint8_t saved) {
    if (curr != saved) {
        char h[3];
        Format::strpadl(h, curr);
        display->print(h, currentX, baseY);
        return true;
    }
    return false;
}

void ClockString::renderContent() {

    display->setColor(textColor);
    display->setBackColor(bgColor);
    display->setFont(Retro8x16);

    if (readTime()) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[ClockString::renderContent] Read done. Render content\n")));

        currentX = baseX;

        if (currentTime.Year != savedTime.Year) {
            uint16_t y = 1970 + currentTime.Year;
            display->printNumI(y, currentX, baseY);
            savedTime.Year = currentTime.Year;
        }
        currentX += (offset * 5);
        if (renderDigitValue(currentTime.Month, savedTime.Month)) {
            savedTime.Month = currentTime.Month;
        }
        currentX += (offset * 3);
        if (renderDigitValue(currentTime.Day, savedTime.Day)) {
            savedTime.Day = currentTime.Day;
        }
        currentX += (offset * 4);

        if (renderDigitValue(currentTime.Hour, savedTime.Hour)) {
            savedTime.Hour = currentTime.Hour;
        }
        currentX += (offset * 3);
        if (renderDigitValue(currentTime.Minute, savedTime.Minute)) {
            savedTime.Minute = currentTime.Minute;
        }
        currentX += (offset * 3);
        if (renderDigitValue(currentTime.Second, savedTime.Second)) {
            savedTime.Second = currentTime.Second;
        }
    } else {
        display->print("Error!", baseX, baseY);
    }
}