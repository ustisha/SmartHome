#include "Task.h"

Task::Task() = default;

int Task::getIndex() {
    uint8_t i = 0;
    do {
        if (a[i].cb == NULL) {
            return i;
        }
    } while (i++ <= MAX);
    return -1;
}

void Task::add(void (*cb)(), uint16_t t, uint8_t type) {
    int i = getIndex();
    if (i > -1) {
        a[i].cb = cb;
        a[i].type = type;
        a[i].last = millis();
        a[i].timeout = t;
    }
}

void Task::each(void (*cb)(), uint16_t t) {
    add(cb, t, TYPE_EACH);
}

void Task::one(void (*cb)(), uint16_t t) {
    add(cb, t, TYPE_ONE);
}

void Task::tick() {
    unsigned long m = millis();
    for (int i = 0; i <= MAX; i++) {
        if (a[i].cb != NULL && m >= (a[i].last + a[i].timeout)) {
            if (a[i].type == TYPE_EACH) {
                a[i].cb();
                a[i].last += a[i].timeout;
            } else if (a[i].type == TYPE_ONE) {
                a[i].cb();
                a[i].cb = NULL;
                a[i].last = 0;
                a[i].timeout = 0;
            }
        }
    }
}

void Task::replace(void (*cb)(), uint16_t t) {
    uint8_t i = 0;
    do {
        if (a[i].cb == cb) {
            a[i].cb = cb;
            a[i].last = millis();
            a[i].timeout = t;
        }
    } while (i++ <= MAX);
}
