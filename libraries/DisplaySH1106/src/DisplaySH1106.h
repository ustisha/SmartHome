#ifndef DISPLAYSH1106_H
#define DISPLAYSH1106_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <U8g2lib.h>
#include <Net.h>
#include <DisplayView.h>
#include <../xbm/led.xbm.h>
#include <../xbm/led_on.xbm.h>

class DisplaySH1106 : public DisplayView {
public:

    DisplaySH1106(uint8_t cs, uint8_t dc, uint8_t rst, const u8g2_cb_t *rotation = U8G2_R0);

   void render() override;

protected:

    U8G2_SH1106_128X64_NONAME_2_4W_HW_SPI *display = nullptr;

    void tempInfo(uint8_t x, uint8_t y, float data);

    void humInfo(uint8_t x, uint8_t y, float data);

    void lightInfo(uint8_t x, uint8_t y, LightController *light, const uint8_t *ledBits = {}, const uint8_t *ledOnBits = {});

};

#endif //DISPLAYSH1106_H
