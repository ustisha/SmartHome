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

const uint8_t PACKET_SIZE = 11;

const uint8_t GATEWAY = 1;
const uint8_t OUTSIDE_TEMP = 2;
const uint8_t GREENHOUSE = 3;

const uint16_t PORT_HTTP_HANDLER = 1001;
const uint16_t PORT_TEMP_CONTROLLER = 1002;
const uint16_t PORT_INFO = 2001;
const uint16_t PORT_18B20 = 2002;
const uint16_t PORT_BME280 = 2003;
const uint16_t PORT_BH1750 = 2004;
const uint16_t PORT_VCC = 2005;
const uint16_t PORT_R1 = 3000;
const uint16_t PORT_R2 = 3001;
const uint16_t PORT_R3 = 3002;
const uint16_t PORT_R4 = 3003;
const uint16_t PORT_R5 = 3004;
const uint16_t PORT_R6 = 3005;
const uint16_t PORT_R7 = 3006;
const uint16_t PORT_R8 = 3007;
const uint16_t PORT_R9 = 3008;
const uint16_t PORT_R10 = 3009;

const uint8_t CMD_TEMPERATURE = 1;
const uint8_t CMD_HUMIDITY = 2;
const uint8_t CMD_PRESSURE = 3;
const uint8_t CMD_LIGHT = 4;
const uint8_t CMD_VCC = 5;

const uint8_t CMD_INFO = 100;
const uint8_t INFO_NETWORK_STARTED = 1;
const uint8_t INFO_SETUP_COMPLETED = 9;
const uint8_t INFO_ERROR_DS18B20 = 20;
const uint8_t INFO_ERROR_BME280 = 21;
const uint8_t INFO_ERROR_BH1750 = 22;

#endif //NET_H
