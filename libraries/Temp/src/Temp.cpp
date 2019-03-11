
#include "Temp.h"

Temp::Temp() = default;

float Temp::get() {
    return temp;
}

int Temp::getStatus() {
    return status;
}

void Temp::setPollInterval(uint16_t tout) {
    timeout = tout * 1000;
}

void Temp::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    if (m >= (last + timeout)) {
        last += timeout;
        IF_SERIAL_DEBUG(printf_P(PSTR("[Temp::tick] Read sensor\n")));
        read();
    }
}