#ifndef BATHROOMWIN_H
#define BATHROOMWIN_H

#include <THPWindow.h>
#include <Lamp.h>

class BathroomWin : public THPWindow {

public:

    Lamp *bath, *toilet;

    BathroomWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);

    void content() override;
};


#endif //BATHROOMWIN_H
