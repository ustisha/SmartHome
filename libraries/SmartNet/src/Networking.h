#ifndef SMARTHOME_NETWORKING_H
#define SMARTHOME_NETWORKING_H

union Int {
    int i = 0;
    uint8_t b[sizeof(int)];
};

union Float {
    float f = 0;
    uint8_t b[sizeof(float)];
};

union Byte {
    byte bt = 0;
    uint8_t b[sizeof(byte)];
};

struct Packet {
    Int sender;
    Int sp;
    Int receiver;
    Int rp;
    uint8_t cmd = NULL;
    Int data;
};

#endif //SMARTHOME_NETWORKING_H
