#ifndef DISPLAYVIEW_H
#define DISPLAYVIEW_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Format.h>
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
};

#endif //DISPLAYVIEW_H
