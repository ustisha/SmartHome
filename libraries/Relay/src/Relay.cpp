#include "Relay.h"

Relay::Relay(uint8_t p, bool invt) : pin(p), invert(invt) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, invert ? HIGH : LOW);
}

void Relay::on() {
    digitalWrite(pin, invert ? LOW : HIGH);
}

void Relay::off() {
    digitalWrite(pin, invert ? HIGH : LOW);
}

void Relay::change() {
    bool s = isOn();
    s ? off() : on();
}

bool Relay::isOn() {
    invert == !digitalRead(pin);
}