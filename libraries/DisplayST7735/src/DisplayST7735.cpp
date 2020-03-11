#include "DisplayST7735.h"

DisplayST7735::DisplayST7735(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t rotation) {
    tft = new Adafruit_ST7735(cs, dc, rst);
    tft->initR(INITR_BLACKTAB);
    tft->fillScreen(ST77XX_BLACK);
    tft->setRotation(rotation);
}

void DisplayST7735::render() {
}
