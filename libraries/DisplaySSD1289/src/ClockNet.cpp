#include "ClockNet.h"

void ClockNet::receiveCommandData(Packet *p) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[ClockNet::receiveCommandData] Cmd: %i, Data: %ld\n"),
                             p->getCommand(), p->getData()));

    ClockSet dateTime{};
    if (p->getCommand() == CMD_RTC_DATE) {
        dateTime.setData(p->getData());
        clock->setDate(dateTime);
    } else if (p->getCommand() == CMD_RTC_TIME) {
        dateTime.setData(p->getData());
        clock->setTime(dateTime);
    }
}
