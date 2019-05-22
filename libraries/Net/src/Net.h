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

// Components
const uint8_t GATEWAY = 1;
const uint8_t OUTSIDE_TEMP = 2;
const uint8_t GREENHOUSE = 3;

// Component ports
const uint16_t PORT_HTTP_HANDLER = 1001;
const uint16_t PORT_TEMP_CONTROLLER = 1002;
const uint16_t PORT_INFO = 2001;
const uint16_t PORT_18B20 = 2002;
const uint16_t PORT_BME280 = 2003;
const uint16_t PORT_BH1750 = 2004;
const uint16_t PORT_VCC = 2005;
const uint16_t PORT_MOISTURE = 2006;

// Commands
const uint8_t CMD_VALUE = 0;
const uint8_t CMD_TEMPERATURE = 1;
const uint8_t CMD_HUMIDITY = 2;
const uint8_t CMD_PRESSURE = 3;
const uint8_t CMD_LIGHT = 4;
const uint8_t CMD_VCC = 5;
const uint8_t CMD_RELAY_00 = 40;
// Reserved for autocalculate relay number
//const uint8_t CMD_RELAY_01 = 41;
//const uint8_t CMD_RELAY_02 = 42;
//const uint8_t CMD_RELAY_03 = 43;
//const uint8_t CMD_RELAY_04 = 44;
//const uint8_t CMD_RELAY_05 = 45;
//const uint8_t CMD_RELAY_06 = 46;
//const uint8_t CMD_RELAY_07 = 47;
//const uint8_t CMD_RELAY_08 = 48;
const uint8_t CMD_RELAY_09 = 49;
// Reserved for autocalculate servo number
const uint8_t CMD_SERVO_00 = 50;
//const uint8_t CMD_SERVO_01 = 51;
//const uint8_t CMD_SERVO_02 = 52;
//const uint8_t CMD_SERVO_03 = 53;
//const uint8_t CMD_SERVO_04 = 54;
//const uint8_t CMD_SERVO_05 = 55;
//const uint8_t CMD_SERVO_06 = 56;
//const uint8_t CMD_SERVO_07 = 57;
//const uint8_t CMD_SERVO_08 = 58;
const uint8_t CMD_SERVO_09 = 59;
const uint8_t CMD_MODE = 11;
const uint8_t CMD_INFO = 100;

// Data values.
const uint8_t RELAY_DISABLED = 0;
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
