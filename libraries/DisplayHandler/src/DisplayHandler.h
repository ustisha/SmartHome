#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

class DisplayView;

#include <Arduino.h>
#include <DebugLog.h>

class DisplayHandler {
public:
    void addDisplayHandler(DisplayView *view);

    void render();

protected:
    DisplayView *displayView = nullptr;
};

#endif //DISPLAYHANDLER_H
