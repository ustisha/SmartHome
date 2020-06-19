#ifndef NET_H
#define NET_H

struct Packet {
    static const uint8_t PACKET_SIZE = 9;

    uint8_t b[PACKET_SIZE];

    uint8_t getSender() {
        return b[0];
    }

    uint8_t getSenderPort() {
        return b[1];
    }

    uint8_t getReceiver() {
        return b[2];
    }

    uint8_t getReceiverPort() {
        return b[3];
    }

    uint8_t getCommand() {
        return b[4];
    }

    long getData() {
        return (long) b[5] << 24 |
               (long) b[6] << 16 |
               (long) b[7] << 8 |
               (long) b[8];
    }

    void setSender(uint8_t sender) {
        b[0] = sender;
    }

    void setSenderPort(uint8_t sp) {
        b[1] = sp;
    }

    void setReceiver(uint8_t receiver) {
        b[2] = receiver;
    }

    void setReceiverPort(uint8_t rp) {
        b[3] = rp;
    }

    void setCommand(uint8_t cmd) {
        b[4] = cmd;
    }

    void setData(long data) {
        b[5] = (uint8_t) (data >> 24 & 0xFF);
        b[6] = (uint8_t) (data >> 16 & 0xFF);
        b[7] = (uint8_t) (data >> 8 & 0XFF);
        b[8] = (uint8_t) (data & 0XFF);
    }
};

// Components
const uint16_t BROADCAST = 255;
const uint16_t GATEWAY = 00;
const uint16_t BATHROOM = 01;
const uint16_t OUTSIDE_TEMP = 02;
const uint16_t GREENHOUSE = 03;
const uint16_t COLD_CHAMBER = 04;

// Component ports
const uint8_t PORT_HTTP_HANDLER = 255;
const uint8_t PORT_INFO = 254;
const uint8_t PORT_18B20 = 2;
const uint8_t PORT_BME280 = 3;
const uint8_t PORT_BH1750 = 4;
const uint8_t PORT_VCC = 5;
const uint8_t PORT_VALUE = 6;
const uint8_t PORT_TEMP_CTRL = 7;
const uint8_t PORT_DHT22 = 9;
const uint8_t PORT_LIGHT_CTRL_00 = 10;
const uint8_t PORT_LIGHT_CTRL_01 = 11;
const uint8_t PORT_LIGHT_CTRL_02 = 12;
const uint8_t PORT_LIGHT_CTRL_03 = 13;
const uint8_t PORT_LIGHT_CTRL_04 = 14;

// Commands
const uint8_t CMD_TEMPERATURE = 1;
const uint8_t CMD_HUMIDITY = 2;
const uint8_t CMD_PRESSURE = 3;
const uint8_t CMD_LIGHT = 4;
const uint8_t CMD_VCC = 5;
const uint8_t CMD_VALUE = 6;
const uint8_t CMD_GET_VALUES = 7;
const uint8_t CMD_MODE = 8;
const uint8_t CMD_TIMEOUT = 9;
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
// Temp controller commands
const uint8_t CMD_UP_LIMIT = 10;
const uint8_t CMD_DOWN_LIMIT = 11;
// Light controller commands
const uint8_t CMD_ACTIVITY_RATIO = 20;
const uint8_t CMD_ACTIVITY_LIMIT = 21;
const uint8_t CMD_RECALL_RATIO = 22;
const uint8_t CMD_RECALL_TIMEOUT = 23;
const uint8_t CMD_TIME_LEFT = 24;
const uint8_t CMD_CALL = 25;
const uint8_t CMD_ENERGY_LEVEL = 30;
// Info
const uint8_t CMD_INFO = 100;

// Data values.
const uint8_t RELAY_DISABLED = 0;
const uint8_t RELAY_ON = 1;
const uint8_t RELAY_OFF = 2;

const uint8_t SERVO_DISABLED = 0;
const uint8_t SERVO_ANGLE = 1;

const uint8_t MODE_AUTO = 1;
const uint8_t MODE_MANUAL = 2;

const uint8_t ENERGY_LVL_OFF = 0;
const uint8_t ENERGY_LVL1 = 1;
const uint8_t ENERGY_LVL2 = 2;

const uint8_t INFO_NETWORK_STARTED = 1;
const uint8_t INFO_DISPLAY_INIT_COMPLETED = 2;
const uint8_t INFO_SETUP_COMPLETED = 9;
const uint8_t INFO_ERROR_DS18B20 = 20;
const uint8_t INFO_ERROR_BME280 = 21;
const uint8_t INFO_ERROR_BH1750 = 22;

#endif //NET_H
