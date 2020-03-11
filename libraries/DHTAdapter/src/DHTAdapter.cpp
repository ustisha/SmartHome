#include <DHTAdapter.h>

DHTAdapter::DHTAdapter(uint8_t pin, uint8_t type) {
    dht = new DHT_nonblocking(pin, type);
    IF_SERIAL_DEBUG(printf_P(PSTR("[DHTAdapter] Begin initialized, pin: %d, type: %d\n"), pin, type));
}

void DHTAdapter::read() {
    reading = true;
    IF_SERIAL_DEBUG(printf_P(PSTR("[DHTAdapter::read] Read started\n")));
}

void DHTAdapter::tick() {
    THInterface::tick();
    if (reading && dht->measure(&temp, &hum)) {
        reading = false;
        render();

#ifdef SERIAL_DEBUG
        char tBuf[8]{};
        Format::floatVar(tBuf, temp);
        char hBuf[8]{};
        Format::floatVar(hBuf, hum);
        IF_SERIAL_DEBUG(printf_P(PSTR("[DHTAdapter::tick] Read done. Temp: %s, Hum: %s\n"), tBuf, hBuf));
#endif
    }
}
