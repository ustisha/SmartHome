#include "BMETempHumPressure.h"

BMETempHumPressure::BMETempHumPressure(BME280 *bme, const uint8_t address) {
    bme->begin(address);
}

void BMETempHumPressure::read() {
    bme.getData(&temp, &pressure, &hum);

#ifdef SERIAL_DEBUG
    String t(temp);
    static char tBuf[8];
    t.toCharArray(tBuf, 8);

    String h(hum);
    static char hBuf[8];
    h.toCharArray(hBuf, 8);

    String p(pressure);
    static char pBuf[8];
    p.toCharArray(pBuf, 8);

    IF_SERIAL_DEBUG(printf_P(PSTR("[BMETempHumPressure::read] Temp: %s, Hum: %s, Pressure %s\n"), tBuf, hBuf, pBuf));
#endif
}
