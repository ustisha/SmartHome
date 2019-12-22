#include "Arduino.h"
#include "Button.h"

Button::Button(uint8_t btnPin) {
    pin = btnPin;
    start = 0;
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
}

uint8_t Button::getIndex() {
    uint8_t i = 0;
    do {
        if (!arr[i].enabled) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

int Button::sortByPress(const void *elem1, const void *elem2) {
    return ((Callback *) elem1)->press < ((Callback *) elem2)->press ? 1 : -1;
}

uint8_t Button::addHandler(HandlerInterface *handlerInterface, uint16_t pressTime) {
    uint8_t i = getIndex();
    if (i >= 0) {
        arr[i].handlerInterface = handlerInterface;
        arr[i].press = pressTime;
        arr[i].enabled = true;
    }
    qsort(arr, MAX, sizeof(Callback), Button::sortByPress);
    return i;
}

bool Button::isPressed() {
    if (pin >= A0) {
        return analogRead(pin) < Button::ANALOG_CONNECTED;
    } else {
        return digitalRead(pin) == 0;
    }
}

void Button::tick() {
    unsigned long m = millis();
    if (start != 0 && start > m) {
        start = m;
    }
    if (isPressed() && start == 0) {
        start = m;
        return;
    }
    if (!isPressed() && start != 0) {
        for (uint8_t i = 0; i < MAX; i++) {
            if (arr[i].enabled && (m - start) >= arr[i].press) {
                arr[i].handlerInterface->call(i);
                break;
            }
        }
        start = 0;
    }
}
