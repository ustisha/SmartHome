#include "Task.h"

Task::Task(uint8_t max) : a(new Callback[max]{}) {
    maxTask = max;
    i = 0;
};

void Task::add(void (*cb)(), uint16_t t, uint8_t type) {
    if (i < maxTask) {
        a[i].cb = cb;
        a[i].type = type;
        a[i].last = millis();
        a[i].timeout = t;
        i++;
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
    for (int x = 0; x < maxTask; x++) {
        if (a[x].cb != NULL && m >= (a[x].last + a[x].timeout)) {
            if (a[x].type == TYPE_EACH) {
                a[x].cb();
                a[x].last += a[x].timeout;
            } else if (a[x].type == TYPE_ONE) {
                a[x].cb();
                a[x].cb = NULL;
                a[x].last = 0;
                a[x].timeout = 0;
            }
        }
    }
}

void Task::replace(void (*cb)(), uint16_t t) {
    uint8_t x = 0;
    do {
        if (a[x].cb == cb) {
            a[x].cb = cb;
            a[x].last = millis();
            a[x].timeout = t;
        }
    } while (x++ < maxTask);
}
