#ifndef COLDCHAMBERWIN_H
#define COLDCHAMBERWIN_H

#include <THPWindow.h>
#include <Lamp.h>

class ColdChamberWin : public THPWindow {

public:

    uint8_t currentProgress;
    Lamp *lamp;

    ColdChamberWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);

    void content() override;

    void renderValve();
};


#endif //COLDCHAMBERWIN_H
