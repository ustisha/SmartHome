#include "SmartNet.h"
#include "LoraNet.h"

SmartNet::SmartNet(uint16_t s) {
    sender = s;
}

void SmartNet::tick() {
    for (int i = 0; i < 2; ++i) {
        // Прием данных
    }
}

//int SmartNet::getIndex() {
//    uint8_t i = 0;
//    do {
//        if (components[i] == NULL) {
//            return i;
//        }
//    } while (i++ <= MAX);
//    return -1;
//}

uint8_t SmartNet::getSender() {
    return sender;
}

