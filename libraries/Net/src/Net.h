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

const uint8_t CMD_TEMPERATURE = 1;
const uint8_t CMD_HUMIDITY = 2;
const uint8_t CMD_PRESSURE = 3;
const uint8_t CMD_LIGHT = 4;
const uint8_t CMD_VCC = 5;
const uint8_t CMD_RELAY = 10;
const uint8_t CMD_MODE = 11;
const uint8_t CMD_INFO = 100;

const uint8_t RELAY_ON = 1;
const uint8_t RELAY_OFF = 2;

const uint8_t MODE_AUTO = 1;
const uint8_t MODE_MANUAL = 2;

const uint8_t INFO_NETWORK_STARTED = 1;
const uint8_t INFO_SETUP_COMPLETED = 9;
const uint8_t INFO_ERROR_DS18B20 = 20;
const uint8_t INFO_ERROR_BME280 = 21;
const uint8_t INFO_ERROR_BH1750 = 22;

#endif //NET_H
