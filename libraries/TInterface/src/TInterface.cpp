
#include "TInterface.h"

TInterface::TInterface() = default;

float TInterface::get() {
    return temp;
}

int TInterface::getStatus() {
    return status;
}

void TInterface::setPollInterval(uint16_t tout) {
    timeout = tout * 1000;
    read();
}

void TInterface::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    if (m >= (last + timeout)) {
        last += timeout;
        IF_SERIAL_DEBUG(printf_P(PSTR("[TInterface::tick] Read sensor\n")));
        read();
    }
    if (m < last) {
        last = m;
    }
}