#include "DisplaySSD1289.h"

DisplaySSD1289::DisplaySSD1289(uint8_t rs, uint8_t wr, uint8_t cs, uint8_t rst) {
    display = new UTFT(ITDB32S, rs, wr, cs, rst);
    display->InitLCD();
    display->clrScr();
    display->fillScr(0x859A);
    home = new HomeWin(display, 2, 22, 103, 70, "HOME");
    home->renderAll();
    outside = new OutsideTempWin(display, 107, 22, 103, 85, "OUTSIDE");
    outside->renderAll();
    coldChamber = new ColdChamberWin(display, 212, 22, 103, 85, "COLD");
    coldChamber->renderAll();
    bathroom = new BathroomWin(display, 2, 96, 103, 40, "BATHROOM");
    bathroom->renderAll();
    clock = new ClockString(display, 10, 3);
    clock->renderContent();
}

void DisplaySSD1289::render() {

    for (int i = 0; i < thIdx; ++i) {
        home->temp = th[i]->get();
        home->hum = th[i]->getHumidity();
    }
    home->renderContent();
}
