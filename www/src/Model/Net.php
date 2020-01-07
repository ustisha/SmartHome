<?php


namespace App\Model;


class Net
{
    // Components
    const BROADCAST = 255;
    const GATEWAY = 00;
    const OUTSIDE_TEMP = 02;
    const GREENHOUSE = 03;
    const COLD_CHAMBER = 04;

    // Component ports
    const PORT_HTTP_HANDLER = 255;
    const PORT_INFO = 254;
    const PORT_18B20 = 2;
    const PORT_BME280 = 3;
    const PORT_BH1750 = 4;
    const PORT_VCC = 5;
    const PORT_VALUE = 6;
    const PORT_TEMP_CTRL = 7;
    const PORT_LIGHT_CTRL_00 = 10;
    const PORT_LIGHT_CTRL_01 = 11;
    const PORT_LIGHT_CTRL_02 = 12;
    const PORT_LIGHT_CTRL_03 = 13;
    const PORT_LIGHT_CTRL_04 = 14;

    const CMD_TEMPERATURE = 1;
    const CMD_HUMIDITY = 2;
    const CMD_PRESSURE = 3;
    const CMD_LIGHT = 4;
    const CMD_VCC = 5;
    const CMD_VALUE = 6;
    const CMD_GET_VALUES = 7;
    const CMD_MODE = 8;
    const CMD_TIMEOUT = 9;
    // Relays
    const CMD_RELAY_00 = 40;
    const CMD_RELAY_01 = 41;
    const CMD_RELAY_02 = 42;
    const CMD_RELAY_03 = 43;
    const CMD_RELAY_04 = 44;
    const CMD_RELAY_05 = 45;
    const CMD_RELAY_06 = 46;
    const CMD_RELAY_07 = 47;
    const CMD_RELAY_08 = 48;
    const CMD_RELAY_09 = 49;
    // Servos
    const CMD_SERVO_00 = 50;
    const CMD_SERVO_01 = 51;
    const CMD_SERVO_02 = 52;
    const CMD_SERVO_03 = 53;
    const CMD_SERVO_04 = 54;
    const CMD_SERVO_05 = 55;
    const CMD_SERVO_06 = 56;
    const CMD_SERVO_07 = 57;
    const CMD_SERVO_08 = 58;
    const CMD_SERVO_09 = 59;
    // Temp controller commands
    const CMD_UP_LIMIT = 10;
    const CMD_DOWN_LIMIT = 11;
    // Light controller commands
    const CMD_ACTIVITY_RATIO = 20;
    const CMD_ACTIVITY_LIMIT = 21;
    const CMD_RECALL_RATIO = 22;
    const CMD_RECALL_TIMEOUT = 23;

    // Info
    const CMD_INFO = 100;

    // Data values.
    const RELAY_DISABLED = 0;
    const RELAY_ON = 1;
    const RELAY_OFF = 2;

    const SERVO_DISABLED = 0;
    const SERVO_ANGLE = 1;

    const MODE_AUTO = 1;
    const MODE_MANUAL = 2;

    const INFO_NETWORK_STARTED = 1;
    const INFO_SETUP_COMPLETED = 9;
    const INFO_ERROR_DS18B20 = 20;
    const INFO_ERROR_BME280 = 21;
    const INFO_ERROR_BH1750 = 22;


    static protected $componentNames = [
        self::OUTSIDE_TEMP => 'outside',
        self::COLD_CHAMBER => 'coldchamber'
    ];

    static protected $portNames = [
        self::PORT_HTTP_HANDLER => 'http_handler',
        self::PORT_TEMP_CTRL => 'temp_controller',
        self::PORT_LIGHT_CTRL_00 => 'light_controller_00',
        self::PORT_LIGHT_CTRL_01 => 'light_controller_01',
        self::PORT_LIGHT_CTRL_02 => 'light_controller_02',
        self::PORT_LIGHT_CTRL_03 => 'light_controller_03',
        self::PORT_LIGHT_CTRL_04 => 'light_controller_04',
        self::PORT_INFO => 'info',
        self::PORT_18B20 => 'ds18b20',
        self::PORT_BME280 => 'bme280',
        self::PORT_BH1750 => 'bh1750',
        self::PORT_VCC => 'value',
        self::PORT_VALUE => 'value',
    ];

    static protected $commandNames = [
        self::CMD_TEMPERATURE => 'temperature',
        self::CMD_HUMIDITY => 'humidity',
        self::CMD_PRESSURE => 'pressure',
        self::CMD_LIGHT => 'light',
        self::CMD_VCC => 'vcc',
        self::CMD_VALUE => 'value',
        self::CMD_GET_VALUES => 'get_values',
        self::CMD_TIMEOUT => 'timeout',
        self::CMD_UP_LIMIT => 'up_limit',
        self::CMD_DOWN_LIMIT => 'down_limit',
        self::CMD_RELAY_00 => 'relay_0',
        self::CMD_RELAY_01 => 'relay_1',
        self::CMD_RELAY_02 => 'relay_2',
        self::CMD_RELAY_03 => 'relay_3',
        self::CMD_RELAY_04 => 'relay_4',
        self::CMD_RELAY_05 => 'relay_5',
        self::CMD_RELAY_06 => 'relay_6',
        self::CMD_RELAY_07 => 'relay_7',
        self::CMD_RELAY_08 => 'relay_8',
        self::CMD_RELAY_09 => 'relay_9',
        self::CMD_SERVO_00 => 'servo_0',
        self::CMD_SERVO_01 => 'servo_1',
        self::CMD_SERVO_02 => 'servo_2',
        self::CMD_SERVO_03 => 'servo_3',
        self::CMD_SERVO_04 => 'servo_4',
        self::CMD_SERVO_05 => 'servo_5',
        self::CMD_SERVO_06 => 'servo_6',
        self::CMD_SERVO_07 => 'servo_7',
        self::CMD_SERVO_08 => 'servo_8',
        self::CMD_SERVO_09 => 'servo_9',
        self::CMD_MODE => 'mode',
        self::CMD_INFO => 'info',
    ];

    static public function getComponentName($id)
    {
        return self::getNameById('componentNames', $id);
    }

    static public function getPortName($id)
    {
        return self::getNameById('portNames', $id);
    }

    static public function getCommandName($id)
    {
        return self::getNameById('commandNames', $id);
    }

    static private function getNameById($arrayName, $id)
    {
        if (isset(self::$$arrayName[$id])) {
            return self::$$arrayName[$id];
        }
        return false;
    }

    static public function nodes()
    {
        return [self::GATEWAY, self::OUTSIDE_TEMP, self::GREENHOUSE];
    }

    static public function ports()
    {
        return [
            self::PORT_HTTP_HANDLER,
            self::PORT_TEMP_CTRL,
            self::PORT_LIGHT_CTRL_00,
            self::PORT_LIGHT_CTRL_01,
            self::PORT_LIGHT_CTRL_02,
            self::PORT_LIGHT_CTRL_03,
            self::PORT_LIGHT_CTRL_04,
            self::PORT_INFO,
            self::PORT_18B20,
            self::PORT_BME280,
            self::PORT_BH1750,
            self::PORT_VALUE
        ];
    }

    static public function commands()
    {
        return [
            self::CMD_VALUE,
            self::CMD_TEMPERATURE,
            self::CMD_HUMIDITY,
            self::CMD_PRESSURE,
            self::CMD_LIGHT,
            self::CMD_VCC,
            self::CMD_INFO,
            self::CMD_RELAY_00,
            self::CMD_RELAY_01,
            self::CMD_RELAY_02,
            self::CMD_RELAY_03,
            self::CMD_RELAY_04,
            self::CMD_RELAY_05,
            self::CMD_RELAY_06,
            self::CMD_RELAY_07,
            self::CMD_RELAY_08,
            self::CMD_RELAY_09,
            self::CMD_SERVO_00,
            self::CMD_SERVO_01,
            self::CMD_SERVO_02,
            self::CMD_SERVO_03,
            self::CMD_SERVO_04,
            self::CMD_SERVO_05,
            self::CMD_SERVO_06,
            self::CMD_SERVO_07,
            self::CMD_SERVO_08,
            self::CMD_SERVO_09,
            self::CMD_MODE,
            self::CMD_INFO,
        ];
    }
}