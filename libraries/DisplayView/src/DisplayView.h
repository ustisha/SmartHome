#ifndef DISPLAYVIEW_H
#define DISPLAYVIEW_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Format.h>
#include <U8g2lib.h>
#include <LightController.h>
#include <THInterface.h>

class DisplayView {

    static const uint8_t MAX_LIGHT_CONTROLLER = 2;
    static const uint8_t MAX_TH = 1;

public:

    void addModule(THInterface *thInterface);

    void addModule(LightController *lightController);

    virtual void render() = 0;

protected:
    LightController *light[MAX_LIGHT_CONTROLLER]{};
    THInterface *th[MAX_TH]{};
    uint8_t thIdx = 0, lIdx = 0;
    U8G2_SH1106_128X64_NONAME_2_4W_HW_SPI *display = nullptr;

    void tempInfo(uint8_t x, uint8_t y, float data);

    void humInfo(uint8_t x, uint8_t y, float data);

    void lightInfo(uint8_t x, uint8_t y, LightController *light, const uint8_t *ledBits = {}, const uint8_t *ledOnBits = {});
};

#endif //DISPLAYVIEW_H
