#include "RF24NetSleep.h"

void RF24NetSleep::sendData(Packet *p) {
    radio.powerUp();
    RF24Net::sendData(p);
    radio.txStandBy();
    radio.powerDown();
}
