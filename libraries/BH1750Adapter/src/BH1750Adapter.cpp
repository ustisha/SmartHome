#include "BH1750Adapter.h"

BH1750Adapter::BH1750Adapter() {
    bh = new BH1750();
    status = (int) bh->begin();
}

void BH1750Adapter::read() {
    light = bh->readLightLevel();

#ifdef SERIAL_DEBUG
    String l(light);
    static char buffer[10];
    l.toCharArray(buffer, 10);

    IF_SERIAL_DEBUG(printf_P(PSTR("[BH1750Adapter::read] Lux: %s\n"), buffer));
#endif
}

int BH1750Adapter::getStatus() {
    return status;
}

