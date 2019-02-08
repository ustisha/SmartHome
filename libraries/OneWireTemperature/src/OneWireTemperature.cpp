#include "OneWireTemperature.h"

OneWireTemperature::OneWireTemperature(OneWire *oneWire, byte idx) : addressIdx(idx) {
    sensors = new DallasTemperature(oneWire);
    sensors->begin();
    sensors->setResolution(11);
}

void OneWireTemperature::read() {
    sensors->requestTemperatures();
    temp = sensors->getTempCByIndex(addressIdx);
    LOG("[OneWire] Temperature: ");
    LOGVLN(temp);
}

float OneWireTemperature::get() {
    return temp;
}
