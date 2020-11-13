#ifndef CLOCK_H
#define CLOCK_H

#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <DS1307RTC.h>

struct ClockSet {

    uint8_t b[sizeof(long)];

    uint8_t getB1() {
        return b[0];
    }

    uint8_t getB2() {
        return b[1];
    }

    uint8_t getB3() {
        return b[2];
    }

    uint8_t getB4() {
        return b[3];
    }

    void setData(long data) {
        b[0] = (uint8_t) (data >> 24 & 0xFF);
        b[1] = (uint8_t) (data >> 16 & 0xFF);
        b[2] = (uint8_t) (data >> 8 & 0XFF);
        b[3] = (uint8_t) (data & 0XFF);
    }
};


class Clock {
public:

    void setDate(ClockSet dateSet);

    void setTime(ClockSet timeSet);

    auto readTime() -> bool;

protected:

    tmElements_t currentTime{0};

    void writeCurrentTime() {
        RTC.write(currentTime);
    }
};


#endif //CLOCK_H
