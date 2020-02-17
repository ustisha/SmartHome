#include "Arduino.h"
#include "Button.h"

Button::Button(uint8_t btnPin, uint8_t max, bool invt) : arr(new Callback[max]{}),
                                                         maxArr(max),
                                                         invert(invt),
                                                         pin(btnPin),
                                                         i(0),
                                                         start(0) {
    digitalWrite(pin, invert ? HIGH : LOW);
    pinMode(pin, INPUT);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Button] Pin: %i, Inversion: %d\n"), pin, (int) invert));
}

auto Button::sortByPress(const void *elem1, const void *elem2) -> int {
    return ((Callback *) elem1)->press < ((Callback *) elem2)->press ? 1 : -1;
}

auto Button::addHandler(HandlerInterface *handlerInterface, uint16_t pressTime) -> int8_t {
    if (i >= maxArr) {
        IF_SERIAL_DEBUG(PSTR("[Button::addHandler] limit reached\n"));
        return -1;
    }
    arr[i].handlerInterface = handlerInterface;
    arr[i].press = pressTime;
    qsort(arr, maxArr, sizeof(Callback), Button::sortByPress);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Button::addHandler] Idx: %i\n"), i));
    return i++;
}

auto Button::isPressed() -> bool {
    if (pin >= A0) {
        return (!invert && analogRead(pin) < Button::ANALOG_CONNECTED) ||
               (invert && analogRead(pin) > Button::ANALOG_CONNECTED);
    } else {
        return (!invert && digitalRead(pin) == 0) || (invert && digitalRead(pin) == 1);
    }
}

void Button::tick() {
    unsigned long m = millis();
    if (start != 0 && start > m) {
        start = m;
    }
    if (isPressed() && start == 0) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Button::tick] Pressed\n")));
        start = m;
        return;
    }
    if (!isPressed() && start != 0) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Button::tick] Released\n")));
        for (uint8_t i = 0; i < maxArr; i++) {
            if (arr[i].handlerInterface != nullptr && (m - start) >= arr[i].press) {
                arr[i].handlerInterface->call(i);
                IF_SERIAL_DEBUG(printf_P(PSTR("[Button::tick] Handler called: %i\n"), i));
                break;
            }
        }
        start = 0;
    }
}
