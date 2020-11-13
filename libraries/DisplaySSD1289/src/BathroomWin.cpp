
#include "BathroomWin.h"

BathroomWin::BathroomWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title) :
        THPWindow(d, x, y, w, h, title) {

    toilet = new Lamp(display, baseX + border + padding + 5, baseY + headerHeight + 4, true);
    bath = new Lamp(display, baseX + border + padding + 52, baseY + headerHeight + 4, true);
}

void BathroomWin::content() {
    THPWindow::content();
    toilet->renderContent();
    bath->renderContent();
}
