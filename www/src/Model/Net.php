<?php


namespace App\Model;


class Net
{
    const GATEWAY = 1;
    const GATEWAY_HTTP_HANDLER = 1001;

    const OUTSIDE_TEMP = 2;
    const OUTSIDE_TEMP_INFO = 2001;
    const OUTSIDE_TEMP_18B20 = 2002;
    const OUTSIDE_TEMP_BME280 = 2003;
    const OUTSIDE_TEMP_BH1750 = 2004;
    const OUTSIDE_TEMP_VCC = 2005;

    const CMD_TEMPERATURE = 1;
    const CMD_HUMIDITY = 2;
    const CMD_PRESSURE = 3;
    const CMD_LIGHT = 4;
    const CMD_VCC = 5;

    const CMD_INFO = 100;
    const INFO_NETWORK_STARTED = 1;
    const INFO_SETUP_COMPLETED = 9;
    const INFO_ERROR_DS18B20 = 20;
    const INFO_ERROR_BME280 = 21;
    const INFO_ERROR_BH1750 = 22;

    static public function nodes()
    {
        return [self::GATEWAY, self::OUTSIDE_TEMP];
    }

    static public function ports()
    {
        return [
            self::GATEWAY_HTTP_HANDLER,
            self::OUTSIDE_TEMP_INFO,
            self::OUTSIDE_TEMP_18B20,
            self::OUTSIDE_TEMP_BME280,
            self::OUTSIDE_TEMP_BH1750,
            self::OUTSIDE_TEMP_VCC,
        ];
    }

    static public function commands()
    {
        return [
            self::CMD_TEMPERATURE,
            self::CMD_HUMIDITY,
            self::CMD_PRESSURE,
            self::CMD_LIGHT,
            self::CMD_VCC,
            self::CMD_INFO
        ];
    }
}