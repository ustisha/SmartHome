#include "BHLight.h"

BHLight::BHLight() {
    bh = new BH1750();
    status = (int) bh->begin();
}

void BHLight::read() {
    light = bh->readLightLevel();

#ifdef SERIAL_DEBUG
    String l(light);
    static char buffer[10];
    l.toCharArray(buffer, 10);

    IF_SERIAL_DEBUG(printf_P(PSTR("[BHLight::read] Lux: %s\n"), buffer));
#endif
}

int BHLight::getStatus() {
    return status;
}

