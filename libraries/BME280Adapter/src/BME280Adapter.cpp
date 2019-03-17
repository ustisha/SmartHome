#include "BME280Adapter.h"

BME280Adapter::BME280Adapter(TwoWire &wire, uint8_t address) {
    bme = new BME280(wire, address);
    status = bme->begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("[BME280Adapter] Status: %d\n"), status));
}

void BME280Adapter::read() {
    bme->readSensor();

    temp = bme->getTemperature_C();
    pressure = bme->getPressure_Pa();
    hum = bme->getHumidity_RH();

    bme->setStandbyTime(BME280::STANDBY_1000_MS);

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

    IF_SERIAL_DEBUG(printf_P(PSTR("[BME280Adapter::read] Temp: %s, Hum: %s, Pressure %s\n"), tBuf, hBuf, pBuf));
#endif
}