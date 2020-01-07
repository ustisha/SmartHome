#include "TInterface.h"

void TInterface::tick(uint16_t sleep) {
    sleepTime += sleep;
    unsigned long m;
    m = millis() + sleepTime;
    if (m >= (last + timeout) || last < timeout) {
        last += timeout;
        IF_SERIAL_DEBUG(printf_P(PSTR("[TInterface::tick] Read sensor\n")));
        read();
    }
    // @todo Проверить как работает таймер при переполнении millis();
    /*if (m < last) {
        last = m;
    }*/
}