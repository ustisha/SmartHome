#ifndef MQTT_MQTTTRANSLATE_H
#define MQTT_MQTTTRANSLATE_H

#include <Net.h>

struct NetMap
{
    const char name[16];
    uint16_t code;
};

const uint8_t COMPONENTS_LEN = 10;
NetMap components[COMPONENTS_LEN] = {
        {"broadcast",      BROADCAST},
        {"www",            WWW},
        {"gateway",        GATEWAY},
        {"bathroom",       BATHROOM},
        {"bathroom_floor", BATHROOM_FLOOR},
        {"outside_temp",   OUTSIDE_TEMP},
        {"outside_temp",   OUTSIDE_TEMP},
        {"greenhouse",     GREENHOUSE},
        {"cold_chamber",   COLD_CHAMBER},
        {"main_display",   MAIN_DISPLAY},
};

const uint8_t PORTS_LEN = 22;
NetMap ports[PORTS_LEN] = {
        {"http_handler",  PORT_HTTP_HANDLER},
        {"info",          PORT_INFO},
        {"18b20",         PORT_18B20},
        {"bme280",        PORT_BME280},
        {"bh1750",        PORT_BH1750},
        {"vcc",           PORT_VCC},
        {"value",         PORT_VALUE},
        {"temp_ctrl",     PORT_TEMP_CTRL},
        {"display",       PORT_DISPLAY},
        {"dht22",         PORT_DHT22},
        {"light_ctrl_00", PORT_LIGHT_CTRL_00},
        {"light_ctrl_01", PORT_LIGHT_CTRL_01},
        {"light_ctrl_02", PORT_LIGHT_CTRL_02},
        {"light_ctrl_03", PORT_LIGHT_CTRL_03},
        {"light_ctrl_04", PORT_LIGHT_CTRL_04},
        {"rtc_clock",     PORT_RTC_CLOCK},
        {"18b20_2",       PORT_18B20_2},
        {"18b20_3",       PORT_18B20_3},
        {"18b20_4 ",      PORT_18B20_4},
        {"temp_ctrl_2",   PORT_TEMP_CTRL_2},
        {"temp_ctrl_3",   PORT_TEMP_CTRL_3},
        {"temp_ctrl_4",   PORT_TEMP_CTRL_4},
};

const uint8_t COMMANDS_LEN = 41;
NetMap commands[COMMANDS_LEN] = {
        {"temperature",    CMD_TEMPERATURE},
        {"humidity",       CMD_HUMIDITY},
        {"pressure",       CMD_PRESSURE},
        {"light",          CMD_LIGHT},
        {"vcc",            CMD_VCC},
        {"value",          CMD_VALUE},
        {"get_values",     CMD_GET_VALUES},
        {"mode",           CMD_MODE},
        {"timeout",        CMD_TIMEOUT},
        {"relay_00",       CMD_RELAY_00},
        {"relay_01",       CMD_RELAY_01},
        {"relay_02",       CMD_RELAY_02},
        {"relay_03",       CMD_RELAY_03},
        {"relay_04",       CMD_RELAY_04},
        {"relay_05",       CMD_RELAY_05},
        {"relay_06",       CMD_RELAY_06},
        {"relay_07",       CMD_RELAY_07},
        {"relay_08",       CMD_RELAY_08},
        {"relay_09",       CMD_RELAY_09},
        {"servo_00",       CMD_SERVO_00},
        {"servo_01",       CMD_SERVO_01},
        {"servo_02",       CMD_SERVO_02},
        {"servo_03",       CMD_SERVO_03},
        {"servo_04",       CMD_SERVO_04},
        {"servo_05",       CMD_SERVO_05},
        {"servo_06",       CMD_SERVO_06},
        {"servo_07",       CMD_SERVO_07},
        {"servo_08",       CMD_SERVO_08},
        {"servo_09",       CMD_SERVO_09},
        {"up_limit",       CMD_UP_LIMIT},
        {"down_limit",     CMD_DOWN_LIMIT},
        {"rtc_date",       CMD_RTC_DATE},
        {"rtc_time",       CMD_RTC_TIME},
        {"activity_ratio", CMD_ACTIVITY_RATIO},
        {"activity_limit", CMD_ACTIVITY_LIMIT},
        {"recall_ratio",   CMD_RECALL_RATIO},
        {"recall_timeout", CMD_RECALL_TIMEOUT},
        {"time_left",      CMD_TIME_LEFT},
        {"call",           CMD_CALL},
        {"energy_level",   CMD_ENERGY_LEVEL},
        {"info",           CMD_INFO},
};

class MqttUtils
{

public:
    void static getName(char *name, uint16 code, NetMap map[], uint8_t len)
    {
        for (uint8_t i = 0; i < len; i++) {
            if (map[i].code == code) {
                strcpy(name, map[i].name);
                return;
            }
        }
    }
    uint16 static getCode(char *name, NetMap map[], uint8_t len)
    {
        for (uint8_t i = 0; i < len; i++) {
            if (strcmp(map[i].name, name) == 0) {
                return map[i].code;
            }
        }
    }
};

#endif //MQTT_MQTTTRANSLATE_H
