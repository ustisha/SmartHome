#ifndef DISPLAYSSD1289_H
#define DISPLAYSSD1289_H

#include <UTFT.h>
#include <Format.h>
#include <DisplayView.h>
#include <ClockString.h>
#include <HomeWin.h>
#include <OutsideTempWin.h>
#include <ColdChamberWin.h>
#include <BathroomWin.h>

class DisplaySSD1289 : public DisplayView {

public:

    ClockString *clock;
    HomeWin *home;
    OutsideTempWin *outside;
    ColdChamberWin *coldChamber;
    BathroomWin *bathroom;

    DisplaySSD1289(uint8_t rs, uint8_t wr, uint8_t cs, uint8_t rst);

    void render() override;

protected:
    UTFT *display = nullptr;
};


#endif //DISPLAYSSD1289_H
