#include <DS18B20Adapter.h>

DS18B20Adapter::DS18B20Adapter(OneWire *oneWire, byte idx) : addressIdx(idx) {
    sensors = new DallasTemperature(oneWire);
    sensors->begin();

    DeviceAddress deviceAddress;
    sensors->getAddress(deviceAddress, idx);
    status = sensors->isConnected(deviceAddress);
    IF_SERIAL_DEBUG(printf_P(PSTR("[DS18B20Adapter] Status: %d\n"), status));

    sensors->setResolution(11);
    sensors->requestTemperatures();
}

void DS18B20Adapter::read() {
    sensors->requestTemperatures();
    temp = sensors->getTempCByIndex(addressIdx);

#ifdef SERIAL_DEBUG
    String t(temp);
    static char buffer[8];
    t.toCharArray(buffer, 8);

    IF_SERIAL_DEBUG(printf_P(PSTR("[DS18B20Adapter::read] Temperature: %s\n"), buffer));
#endif
}
