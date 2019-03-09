#include "BMETempHumPressure.h"

BMETempHumPressure::BMETempHumPressure(TwoWire &wire, uint8_t address) {
    bme = new BME280(wire, address);
    status = bme->begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("[BMETempHumPressure] Status: %d\n"), status));
}

void BMETempHumPressure::read() {
    bme->readSensor();

    temp = bme->getTemperature_C();
    pressure = bme->getPressure_Pa();
    hum = bme->getHumidity_RH();

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
