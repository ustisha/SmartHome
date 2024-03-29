#ifndef MQTT_MQTTTRANSLATE_H
#define MQTT_MQTTTRANSLATE_H

#include <Net.h>

struct NetMap
{
    const char name[32];
    uint16_t code;
};

const uint8_t COMPONENTS_LEN = 10;
NetMap const components[COMPONENTS_LEN] = {
        {"broadcast",      BROADCAST},
        {"www",            WWW},
        {"gateway",        GATEWAY},
        {"bathroom",       BATHROOM},
        {"bathroom_floor", BATHROOM_FLOOR},
        {"bathroom_valve", BATHROOM_VALVE},
        {"cold_chamber",   COLD_CHAMBER},
        {"outside_temp",   OUTSIDE_TEMP},
        {"greenhouse",     GREENHOUSE},
        {"garage",         GARAGE},
};

// Remove prefix port_ !
const uint8_t PORTS_LEN = 63;
NetMap const ports[PORTS_LEN] = {
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
        {"motion1",       PORT_MOTION_1},
        {"motion2",       PORT_MOTION_2},
        {"motion3",       PORT_MOTION_3},
        {"motion4",       PORT_MOTION_4},
        {"motion5",       PORT_MOTION_5},
        {"18b20_2",       PORT_18B20_2},
        {"18b20_3",       PORT_18B20_3},
        {"18b20_4",       PORT_18B20_4},
        {"washing_machine", PORT_WASHING_MACHINE},
        {"tvirtual_1",  PORT_TVIRTUAL_1},
        {"tvirtual_2",  PORT_TVIRTUAL_2},
        {"tvirtual_3",  PORT_TVIRTUAL_3},
        {"tvirtual_4",  PORT_TVIRTUAL_4},
        {"servo_00",    PORT_SERVO_00},
        {"servo_01",    PORT_SERVO_01},
        {"servo_02",    PORT_SERVO_02},
        {"servo_03",    PORT_SERVO_03},
        {"servo_04",    PORT_SERVO_04},
        {"relay_00",    PORT_RELAY_00},
        {"relay_01",    PORT_RELAY_01},
        {"relay_02",    PORT_RELAY_02},
        {"relay_03",    PORT_RELAY_03},
        {"relay_04",    PORT_RELAY_04},
        {"relay_05",    PORT_RELAY_05},
        {"relay_06",    PORT_RELAY_06},
        {"relay_07",    PORT_RELAY_07},
        {"relay_08",    PORT_RELAY_08},
        {"relay_09",    PORT_RELAY_09},
        {"dimmer_00",   PORT_DIMMER_00},
        {"dimmer_01",   PORT_DIMMER_01},
        {"dimmer_02",   PORT_DIMMER_02},
        {"dimmer_03",   PORT_DIMMER_03},
        {"dimmer_04",   PORT_DIMMER_04},
        {"dimmer_05",   PORT_DIMMER_05},
        {"value_2",     PORT_VALUE_2},
        {"value_3",     PORT_VALUE_3},
        {"value_4",     PORT_VALUE_4},
        {"value_5",     PORT_VALUE_5},
        {"value_6",     PORT_VALUE_6},
        {"value_7",     PORT_VALUE_7},
        {"value_8",     PORT_VALUE_8},
        {"value_9",     PORT_VALUE_9},
        {"temp_ctrl_2", PORT_TEMP_CTRL_2},
        {"temp_ctrl_3", PORT_TEMP_CTRL_3},
        {"temp_ctrl_4", PORT_TEMP_CTRL_4},
        {"temp_ctrl_group_1", PORT_TEMP_CTRL_GROUP_1},
        {"temp_ctrl_group_2", PORT_TEMP_CTRL_GROUP_2},
};

// Remove prefix cmd_ !
const uint8_t COMMANDS_LEN = 71;
NetMap const commands[COMMANDS_LEN] = {
        {"temperature",        CMD_TEMPERATURE},
        {"humidity",           CMD_HUMIDITY},
        {"pressure",           CMD_PRESSURE},
        {"light",              CMD_LIGHT},
        {"vcc",                CMD_VCC},
        {"value",              CMD_VALUE},
        {"get_values",         CMD_GET_VALUES},
        {"mode",               CMD_MODE},
        {"timeout",            CMD_TIMEOUT},
        {"relay_00",           CMD_RELAY_00},
        {"relay_01",           CMD_RELAY_01},
        {"relay_02",           CMD_RELAY_02},
        {"relay_03",           CMD_RELAY_03},
        {"relay_04",           CMD_RELAY_04},
        {"relay_on_00",        CMD_RELAY_ON_00},
        {"relay_on_01",        CMD_RELAY_ON_01},
        {"relay_on_02",        CMD_RELAY_ON_02},
        {"relay_on_03",        CMD_RELAY_ON_03},
        {"relay_on_04",        CMD_RELAY_ON_04},
        {"relay_off_00",       CMD_RELAY_OFF_00},
        {"relay_off_01",       CMD_RELAY_OFF_01},
        {"relay_off_02",       CMD_RELAY_OFF_02},
        {"relay_off_03",       CMD_RELAY_OFF_03},
        {"relay_off_04",       CMD_RELAY_OFF_04},
        {"servo_00",           CMD_SERVO_00},
        {"servo_01",           CMD_SERVO_01},
        {"servo_02",           CMD_SERVO_02},
        {"servo_03",           CMD_SERVO_03},
        {"servo_04",           CMD_SERVO_04},
        {"servo_min_angle_00", CMD_SERVO_MIN_ANGLE_00},
        {"servo_min_angle_01", CMD_SERVO_MIN_ANGLE_01},
        {"servo_min_angle_02", CMD_SERVO_MIN_ANGLE_02},
        {"servo_min_angle_03", CMD_SERVO_MIN_ANGLE_03},
        {"servo_min_angle_04", CMD_SERVO_MIN_ANGLE_04},
        {"servo_max_angle_00", CMD_SERVO_MAX_ANGLE_00},
        {"servo_max_angle_01", CMD_SERVO_MAX_ANGLE_01},
        {"servo_max_angle_02", CMD_SERVO_MAX_ANGLE_02},
        {"servo_max_angle_03", CMD_SERVO_MAX_ANGLE_03},
        {"servo_max_angle_04", CMD_SERVO_MAX_ANGLE_04},
        {"servo_ratio_00",     CMD_SERVO_RATIO_00},
        {"servo_ratio_01",     CMD_SERVO_RATIO_01},
        {"servo_ratio_02",     CMD_SERVO_RATIO_02},
        {"servo_ratio_03",     CMD_SERVO_RATIO_03},
        {"servo_ratio_04",     CMD_SERVO_RATIO_04},
        {"up_limit",           CMD_UP_LIMIT},
        {"down_limit",         CMD_DOWN_LIMIT},
        {"rtc_date",           CMD_RTC_DATE},
        {"rtc_time",           CMD_RTC_TIME},
        {"relay",              CMD_RELAY},
        {"moisture",              CMD_MOISTURE},
        {"motion",                CMD_MOTION},
        {"washing_state",         CMD_WASHING_STATE},
        {"washing_remaining_sec", CMD_WASHING_REMAINING_SEC},
        {"washing_door_opened",   CMD_WASHING_DOOR_OPENED},
        {"activity_ratio",        CMD_ACTIVITY_RATIO},
        {"activity_limit",        CMD_ACTIVITY_LIMIT},
        {"recall_ratio",          CMD_RECALL_RATIO},
        {"recall_timeout",        CMD_RECALL_TIMEOUT},
        {"time_left",             CMD_TIME_LEFT},
        {"call",                  CMD_CALL},
        {"energy_level",          CMD_ENERGY_LEVEL},
        {"state",                 CMD_STATE},
        {"servo_angle",           CMD_SERVO_ANGLE},
        {"servo_percent",         CMD_SERVO_PERCENT},
        {"servo_min_angle",       CMD_SERVO_MIN_ANGLE},
        {"servo_max_angle",       CMD_SERVO_MAX_ANGLE},
        {"servo_speed",           CMD_SERVO_SPEED},
        {"servo_move_type",       CMD_SERVO_MOVE_TYPE},
        {"map_min",               CMD_MAP_MIN},
        {"map_max",               CMD_MAP_MAX},
        {"info",                  CMD_INFO},
};

const uint8_t FLOAT_COMMANDS_LEN = 24;
uint8_t const floatCommands[FLOAT_COMMANDS_LEN] = {
        CMD_TEMPERATURE,
        CMD_HUMIDITY,
        CMD_PRESSURE,
        CMD_LIGHT,
        CMD_VCC,

        CMD_RELAY_ON_00,
        CMD_RELAY_ON_01,
        CMD_RELAY_ON_02,
        CMD_RELAY_ON_03,
        CMD_RELAY_ON_04,

        CMD_RELAY_OFF_00,
        CMD_RELAY_OFF_01,
        CMD_RELAY_OFF_02,
        CMD_RELAY_OFF_03,
        CMD_RELAY_OFF_04,

        CMD_SERVO_RATIO_00,
        CMD_SERVO_RATIO_01,
        CMD_SERVO_RATIO_02,
        CMD_SERVO_RATIO_03,
        CMD_SERVO_RATIO_04,

        CMD_DOWN_LIMIT,
        CMD_UP_LIMIT,

        CMD_ACTIVITY_RATIO,
        CMD_RECALL_RATIO
};

class MqttUtils
{

public:
    bool static getName(char *name, uint16 code, const NetMap map[], uint8_t len)
    {
        for (uint8_t i = 0; i < len; i++) {
            if (map[i].code == code) {
                strcpy(name, map[i].name);
                return true;
            }
        }
        return false;
    }

    int32_t static getCode(char *name, const NetMap *map, uint8_t len)
    {
        for (uint8_t i = 0; i < len; i++) {
            if (strcmp(map[i].name, name) == 0) {
                return map[i].code;
            }
        }
        return -1;
    }

    bool static isFloat(uint8_t cmd, const uint8_t map[], uint8_t len)
    {
        for (uint8_t i = 0; i < len; i++) {
            if (map[i] == cmd) {
                return true;
            }
        }
        return false;
    }
};

#endif //MQTT_MQTTTRANSLATE_H
