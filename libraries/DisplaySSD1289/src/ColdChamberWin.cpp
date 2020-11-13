#include "ColdChamberWin.h"

ColdChamberWin::ColdChamberWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title)
        : THPWindow(d, x, y, w, h, title) {
    currentProgress = 0;
    lamp = new Lamp(display, baseX + 55, baseY + 49);
}

void ColdChamberWin::content() {
    THPWindow::content();
    renderValve();
    lamp->renderContent();
}

void ColdChamberWin::renderValve() {
    progressBar(baseY + 48, 10, currentProgress);
}
