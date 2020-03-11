#ifndef DISPLAYSH1106_H
#define DISPLAYSH1106_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Net.h>
#include <DisplayView.h>
#include <../xbm/led.xbm.h>
#include <../xbm/led_on.xbm.h>

class DisplaySH1106 : public DisplayView {
public:

    DisplaySH1106(uint8_t cs, uint8_t dc, uint8_t rst, const u8g2_cb_t *rotation = U8G2_R0);

   void render() override;
};

#endif //DISPLAYSH1106_H
