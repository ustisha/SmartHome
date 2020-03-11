#ifndef DISPLAYST7735_H
#define DISPLAYST7735_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Net.h>
#include <DisplayView.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeSans24pt7b.h>

class DisplayST7735 : public DisplayView {
public:

    DisplayST7735(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t rotation = 1);

    void render() override;

protected:
    Adafruit_ST7735 *tft;
};

#endif //DISPLAYST7735_H
