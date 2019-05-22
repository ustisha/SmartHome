#include "AnalogReader.h"

AnalogReader::AnalogReader(uint8_t p) : pin(p) {
    pinMode(pin, INPUT);
}

int AnalogReader::read() {
    return analogRead(pin);
}
