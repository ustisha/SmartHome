#ifndef OUTSIDETEMPWIN_H
#define OUTSIDETEMPWIN_H

#include <THPWindow.h>

class OutsideTempWin : public THPWindow {

public:
    OutsideTempWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);
};


#endif //OUTSIDETEMPWIN_H
