#include "THPWindow.h"

THPWindow::THPWindow(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title) :
        DisplayWindow(d, x, y, w, h, title) {
    tempFontSize = 32;
    humFontSize = 16;
    presFontSize = 16;
}

void THPWindow::content() {
    IF_SERIAL_DEBUG(PSTR("[THPWindow::content] rendering\n"));

    display->setColor(textColor);
    display->setBackColor(bgColor);
    if (temp != UNUSED) {
        tempInfo(baseX + border + padding, baseY + headerHeight + padding, temp);
    }
    if (hum != UNUSED) {
        humInfo(baseX + border + padding, baseY + headerHeight + padding + tempFontSize, hum);
    }
    if (pressure != UNUSED) {
        pressureInfo(baseX + border + padding, baseY + headerHeight + padding + tempFontSize + humFontSize, pressure);
    }
}

void THPWindow::tempInfo(uint8_t x, uint8_t y, float data) {
    char temp[8];
    Format::temperature(temp, data, true, "");
    display->setFont(Retro16x32);
    display->print(temp, x, y);
}

void THPWindow::humInfo(uint8_t x, uint8_t y, float data) {
    char hum[8];
    Format::humidity(hum, data, 1);
    display->setFont(Retro8x16);
    display->print(hum, x, y);
}

void THPWindow::pressureInfo(uint8_t x, uint8_t y, float data) {
    char pressure[12];
    Format::pressure(pressure, data);
    display->setFont(Retro8x16);
    display->print(pressure, x, y);
}
