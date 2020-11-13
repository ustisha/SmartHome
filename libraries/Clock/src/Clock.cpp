#include "Clock.h"

bool Clock::readTime() {
    if (RTC.read(currentTime)) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Clock::readTime] Read done. Render content\n")));
        return true;
    } else {
        if (RTC.chipPresent()) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Clock::readTime] The DS1307 is stopped.  Please run the SetTime\n")));
        } else {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Clock::readTime] DS1307 read error!  Please check the circuitry.\n")));
        }
    }
    return false;
}

void Clock::setDate(ClockSet dateSet) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Clock::setDate] Year: %d, Month: %d, Day: %d\n"),
                             dateSet.getB1(), dateSet.getB2(), dateSet.getB3()));
    currentTime.Year = dateSet.getB1();
    currentTime.Month = dateSet.getB2();
    currentTime.Day = dateSet.getB3();
    writeCurrentTime();
}

void Clock::setTime(ClockSet timeSet) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Clock::setDate] Hour: %d, Minute: %d, Second: %d\n"),
                             timeSet.getB1(), timeSet.getB2(), timeSet.getB3()));
    currentTime.Hour = timeSet.getB1();
    currentTime.Minute = timeSet.getB2();
    currentTime.Second = timeSet.getB3();
    writeCurrentTime();
}
