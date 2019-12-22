#include "RF24NetSleep.h"

void RF24NetSleep::sendData(Packet *p) {
    radio.powerUp();
    RF24Net::sendData(p);
    radio.txStandBy();
    radio.powerDown();
}

void RF24NetSleep::receiveData(Packet *p) {
    RF24Net::receiveData(p);
}