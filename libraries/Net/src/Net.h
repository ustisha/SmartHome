#ifndef NET_H
#define NET_H

struct Packet {
    uint8_t sender;
    uint16_t sp;
    uint8_t receiver;
    uint16_t rp;
    uint8_t cmd;
    long data;
};

const uint8_t GATEWAY = 1;
const uint16_t GATEWAY_HTTP_HANDLER = 1001;

const uint8_t OUTSIDE_TEMP = 2;
const uint16_t OUTSIDE_TEMP_18B20 = 2001;
const uint16_t OUTSIDE_TEMP_BME280 = 2002;

const uint8_t CMD_TEMPERATURE = 1;
const uint8_t CMD_HUMIDITY = 2;
const uint8_t CMD_PRESSURE = 3;

#endif //NET_H