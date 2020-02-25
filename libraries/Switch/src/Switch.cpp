#include "Switch.h"

void Switch::tick() {
    unsigned long m = millis();
    uint16_t maxPress = 0;
    if (start != 0 && start > m) {
        start = m;
    }
    // Switch was triggered.
    if (isPressed() && start == 0) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Switch::tick] Pressed\n")));
        start = m;
        return;
    }
    // Switch was pressed, check handlers.
    if (start != 0) {
        for (uint8_t i = 0; i < maxArr; i++) {
            // Search max long press time.
            if (arr[i].press > maxPress) {
                maxPress = arr[i].press;
            }
            // Press time reached.
            if (!wait && arr[i].handlerInterface != nullptr && (m - start) >= arr[i].press) {
                arr[i].handlerInterface->call(arr[i].type, arr[i].idx);
                IF_SERIAL_DEBUG(printf_P(PSTR("[Switch::tick] Handler called: %i\n"), i));
                break;
            }
        }
        // All handlers executed.
        if ((m - start) >= maxPress) {
            wait = true;
            // Switch state was changed reset start time.
            if (!isPressed()) {
                IF_SERIAL_DEBUG(printf_P(PSTR("[Switch::tick] Start reset\n")));
                start = 0;
                wait = false;
            }
        }
    }
}
