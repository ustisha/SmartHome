#ifndef HOMEWIN_H
#define HOMEWIN_H

#include <THPWindow.h>

class HomeWin : public THPWindow {

public:

    HomeWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);
};


#endif //HOMEWIN_H
