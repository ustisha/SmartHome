#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI *oled;

void setup(void) {
    Serial.begin(57600);
    Serial.println("Setup done");

    oled = new U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI(U8G2_R0, 10, 9, 8);
    bool status = oled->begin();

    oled->clearDisplay();
    oled->setFont(u8g2_font_tenfatguys_tf);
    oled->drawUTF8(2, 12, "HELLO!");
    oled->sendBuffer();
}

void loop(void) {

}

