#ifndef SWITCH_H
#define SWITCH_H

#include <Button.h>

class Switch : public Button {

protected:

    bool wait = false;

public:
    using Button::Button;

    void tick();
};


#endif //SWITCH_H
