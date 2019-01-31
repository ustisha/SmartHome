#include "OneWireTemperature.h"

OneWireTemperature::OneWireTemperature(OneWire *oneWire, byte idx) : addressIdx(idx) {
    sensors = new DallasTemperature(oneWire);
    sensors->begin();
}

void OneWireTemperature::read() {
    temp = sensors->getTempCByIndex(addressIdx);
}

float OneWireTemperature::get() {
    return temp;
}
