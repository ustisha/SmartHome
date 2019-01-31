#ifndef ARDUINOEXAMPLE_TASK_H
#define ARDUINOEXAMPLE_TASK_H

#include <Arduino.h>

class Task {
    const static uint8_t MAX = 4;
    const static uint8_t TYPE_EACH = 1;
    const static uint8_t TYPE_ONE = 2;

    struct Callback {
        void (*cb)() = NULL;

        uint8_t type = 0;
        unsigned long last = 0;
        uint16_t timeout = 0;
    };

    Callback a[MAX]{};
protected:
    int getIndex();

    void add(void (*cb)(), uint16_t t, uint8_t type);

public:
    Task();

    void each(void (*cb)(), uint16_t t);

    void one(void (*cb)(), uint16_t t);

    void replace(void (*cb)(), uint16_t t);

    void tick();
};


#endif //ARDUINOEXAMPLE_TASK_H
