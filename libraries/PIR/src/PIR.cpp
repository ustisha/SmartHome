#include "PIR.h"

void PIR::tick() {
    if (millis() < detectDelay) {
        return;
    }
    Switch::tick();
}
