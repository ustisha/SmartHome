#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

//#define SERIAL_DEBUG

#include <UTFT.h>
#include <DebugLog.h>
#include <Format.h>

extern uint8_t Retro8x16[];
extern uint8_t Retro16x32[];
extern uint8_t mykefont2[];

class DisplayWindow {

public:
    DisplayWindow(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);

    void renderAll();

    void renderContent();

    virtual void content() = 0;

protected:

    UTFT *display;
    uint16_t baseX, baseY, baseW, baseH;
    const char *baseTitle;
    word borderColor, bgColor, titleColor, textColor;
    word progressColor, progressBgColor;
    uint8_t border, headerHeight, padding;

    void borderRender();

    void progressBar(uint16_t y, uint16_t h, uint8_t progress);
};


#endif //DISPLAYWINDOW_H
