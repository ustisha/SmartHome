#ifndef SMARTHOME_NETWORKING_H
#define SMARTHOME_NETWORKING_H

union UInt {
    int i = 0;
    uint8_t b[sizeof(unsigned int)];
};

union Long {
    int i = 0;
    uint8_t b[sizeof(long)];
};

struct Packet {
    UInt sender;
    UInt sp;
    UInt receiver;
    UInt rp;
    UInt cmd;
    Long data;
};

const uint16_t GATEWAY = 1;
const uint16_t GATEWAY_HTTP_HANDLER = 1001;

const uint16_t OUTSIDE_TEMP = 2;
const uint16_t OUTSIDE_TEMP_18B20 = 2001;
const uint16_t OUTSIDE_TEMP_CMD_TEMPERATURE = 1;

#endif //SMARTHOME_NETWORKING_H
