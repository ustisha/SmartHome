#ifndef THPWINDOW_H
#define THPWINDOW_H

#define SERIAL_DEBUG

#include <Format.h>
#include <DisplayWindow.h>

class THPWindow : public DisplayWindow {

    const float UNUSED = -222;
public:

    float temp = UNUSED;
    float hum = UNUSED;
    float pressure = UNUSED;

    THPWindow(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);

    void content() override;

protected:

    uint8_t tempFontSize, humFontSize, presFontSize;

    void tempInfo(uint8_t x, uint8_t y, float data);

    void humInfo(uint8_t x, uint8_t y, float data);

    void pressureInfo(uint8_t x, uint8_t y, float data);
};


#endif //THPWINDOW_H
