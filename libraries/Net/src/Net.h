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
const uint16_t OUTSIDE_TEMP_INFO = 2001;
const uint16_t OUTSIDE_TEMP_18B20 = 2002;
const uint16_t OUTSIDE_TEMP_BME280 = 2003;
const uint16_t OUTSIDE_TEMP_BH1750 = 2004;

const uint8_t CMD_TEMPERATURE = 1;
const uint8_t CMD_HUMIDITY = 2;
const uint8_t CMD_PRESSURE = 3;
const uint8_t CMD_LIGHT = 4;

const uint8_t CMD_INFO = 100;
const uint8_t INFO_NETWORK_STARTED = 1;
const uint8_t INFO_SETUP_COMPLETED = 9;
const uint8_t INFO_ERROR_18B20 = 20;
const uint8_t INFO_ERROR_BME280 = 21;
const uint8_t INFO_ERROR_BH1750 = 22;

#endif //NET_H
