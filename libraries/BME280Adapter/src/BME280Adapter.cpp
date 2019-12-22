#include <Format.h>
#include "BME280Adapter.h"

BME280Adapter::BME280Adapter(TwoWire &wire, uint8_t address) {
    bme = new BME280(wire, address);
    status = bme->begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("[BME280Adapter] Begin status: %d\n"), status));
}

void BME280Adapter::read() {
    status = bme->readSensor();
    IF_SERIAL_DEBUG(printf_P(PSTR("[BME280Adapter::read] Read status: %d\n"), status));

    if (status != 1) {
        status = bme->begin();
        IF_SERIAL_DEBUG(printf_P(PSTR("[BME280Adapter::read] Failed! Begin status: %d\n"), status));
        if (!status) {
            return;
        }
    }

    bme->setStandbyTime(BME280::STANDBY_1000_MS);

    temp = bme->getTemperature_C();
    pressure = bme->getPressure_Pa();
    hum = bme->getHumidity_RH();

#ifdef SERIAL_DEBUG
    char tBuf[8]{};
    Format::floatVar(tBuf, temp);
    char hBuf[8]{};
    Format::floatVar(hBuf, hum);
    char pBuf[10]{};
    Format::floatVar(pBuf, pressure);
    IF_SERIAL_DEBUG(printf_P(PSTR("[BME280Adapter::read] Temp: %s, Hum: %s, Pressure %s\n"), tBuf, hBuf, pBuf));
#endif
}
