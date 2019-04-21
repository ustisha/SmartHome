<?php


namespace App\Model;


use App\Entity\RadioLog;
use Doctrine\ORM\EntityManagerInterface;

class OutsideTemp implements \JsonSerializable
{
    const CURRENT = 1;
    const MAX = 2;
    const MIN = 3;
    const LAST_HOUR = 4;
    const EACH_HOUR = 5;

    /**
     * @var array
     */
    protected $temperature = [];

    /**
     * @var array
     */
    protected $humidity = [];

    /**
     * Pressure in Pa
     *
     * @var array
     */
    protected $pressure = [];

    /**
     * In Lux
     *
     * @var int|null
     */
    protected $lux;

    protected $vcc;

    protected $lastUpdate;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $commands = [Net::CMD_TEMPERATURE, Net::CMD_HUMIDITY, Net::CMD_PRESSURE, Net::CMD_LIGHT, Net::CMD_VCC];
        $data = $entityManager->getRepository(RadioLog::class)->senderLog(Net::OUTSIDE_TEMP, $commands);
        $this->lastUpdate = new \DateTime();
        foreach ($data as $outside) {
            if ($outside->getCommand() == Net::CMD_TEMPERATURE) {
                $this->temperature[self::CURRENT][$outside->getSenderPort()] = $outside->getData() / 100;
            } elseif ($outside->getCommand() == Net::CMD_HUMIDITY) {
                $this->humidity[self::CURRENT] = $outside->getData() / 100;
            } elseif ($outside->getCommand() == Net::CMD_PRESSURE) {
                $this->pressure[self::CURRENT] = $outside->getData();
            } elseif ($outside->getCommand() == Net::CMD_LIGHT) {
                $this->lux = $outside->getData();
            } elseif ($outside->getCommand() == Net::CMD_VCC) {
                $this->vcc = $outside->getData() / 100;
            }
            if ($this->lastUpdate > $outside->getDate()) {
                $this->lastUpdate = $outside->getDate();
            }
        }

        $from = (new \DateTime())->setTime(9, 0, 0);
        $to = (new \DateTime())->setTime(23, 59, 59);
        $data = $entityManager->getRepository(RadioLog::class)->loadMaxValues(
            Net::OUTSIDE_TEMP,
            $from,
            $to,
            [Net::CMD_TEMPERATURE, Net::CMD_HUMIDITY]
        );
        foreach ($data as $row) {
            if ($row->getCommand() == Net::CMD_TEMPERATURE) {
                $this->temperature[self::MAX][$row->getSenderPort()] = $row->getData() / 100;
            } elseif ($row->getCommand() == Net::CMD_HUMIDITY) {
                $this->humidity[self::MAX] = $row->getData() / 100;
            }
        }

        $from = (new \DateTime())->setTime(0, 0, 0);
        $to = (new \DateTime())->setTime(8, 59, 59);
        $data = $entityManager->getRepository(RadioLog::class)->loadMinValues(
            Net::OUTSIDE_TEMP,
            $from,
            $to,
            [Net::CMD_TEMPERATURE, Net::CMD_HUMIDITY]
        );
        foreach ($data as $row) {
            if ($row->getCommand() == Net::CMD_TEMPERATURE) {
                $this->temperature[self::MIN][$row->getSenderPort()] = $row->getData() / 100;
            } elseif ($row->getCommand() == Net::CMD_HUMIDITY) {
                $this->humidity[self::MIN] = $row->getData() / 100;
            }
        }

        $from = (new \DateTime('-1 hour'));
        $to = (new \DateTime('-55 min'));
        $data = $entityManager->getRepository(RadioLog::class)->loadAvgValues(
            Net::OUTSIDE_TEMP,
            $from,
            $to,
            [Net::CMD_TEMPERATURE, Net::CMD_HUMIDITY]
        );
        foreach ($data as $row) {
            if ($row->getCommand() == Net::CMD_TEMPERATURE) {
                $this->temperature[self::LAST_HOUR][$row->getSenderPort()] = $row->getData() / 100;
            } elseif ($row->getCommand() == Net::CMD_HUMIDITY) {
                $this->humidity[self::LAST_HOUR] = $row->getData() / 100;
            }
        }

        foreach ([1, 2, 3, 6, 12] as $i) {
            $from = (new \DateTime("-$i hour"));
            $toHour = $i - 1;
            $to = (new \DateTime("-$toHour hour -55 min"));
            $data = $entityManager->getRepository(RadioLog::class)->loadAvgValues(
                Net::OUTSIDE_TEMP,
                $from,
                $to,
                [Net::CMD_PRESSURE]
            );
            foreach ($data as $row) {
                $this->pressure[self::EACH_HOUR][$i] = $row->getData();
            }
        }
    }

    /**
     * @param int $sensor
     *
     * @param int $type
     *
     * @return float|null
     */
    private function getTemp(int $sensor, int $type)
    {
        if (isset($this->temperature[$type][$sensor])) {
            return $this->temperature[$type][$sensor];
        }
        return null;
    }

    /**
     * @return float|null
     */
    public function getTemperature()
    {
        return $this->getTemperatureDS18B20();
    }

    /**
     * @param int $type
     *
     * @return float|null
     */
    public function getTemperatureDS18B20($type = self::CURRENT)
    {
        return $this->getTemp(Net::OUTSIDE_TEMP_18B20, $type);
    }

    /**
     * @param int $type
     *
     * @return float|null
     */
    public function getTemperatureBME280($type = self::CURRENT)
    {
        return $this->getTemp(Net::OUTSIDE_TEMP_BME280, $type);
    }

    /**
     * @param int $type
     *
     * @return float|null
     */
    public function getHumidity($type = self::CURRENT)
    {
        if (isset($this->humidity[$type])) {
            return $this->humidity[$type];
        }
        return null;
    }

    private function convertPressureToMmHg($value)
    {
        return round($value / 13332.2387415, 2);
    }

    public function getPressure($type = self::CURRENT)
    {
        if (isset($this->pressure[$type])) {
            return $this->pressure[$type];
        }
        return null;
    }

    public function getPressureMmHg($type = self::CURRENT)
    {
        if (is_array($this->getPressure($type))) {
            $arr = [];
            $input = $this->getPressure($type);
            krsort($input);
            foreach ($input as $key => $value) {
                $arr["{$key}h"] = $this->convertPressureToMmHg($value);
            }
            $arr["0h"] = $this->getPressureMmHg(self::CURRENT);
            return $arr;
        } elseif (isset($this->pressure[$type])) {
            return $this->convertPressureToMmHg($this->pressure[$type]);
        }
        return null;
    }

    /**
     * @return int|null
     */
    public function getLux(): ?int
    {
        return $this->lux;
    }

    /**
     * @return float|int
     */
    public function getVcc()
    {
        return $this->vcc;
    }

    /**
     * @return \DateTimeInterface|null
     */
    public function getLastUpdate(): ?\DateTimeInterface
    {
        return $this->lastUpdate;
    }

    /**
     * Specify data which should be serialized to JSON
     * @link  https://php.net/manual/en/jsonserializable.jsonserialize.php
     * @return mixed data which can be serialized by <b>json_encode</b>,
     * which is a value of any type other than a resource.
     * @since 5.4.0
     */
    public function jsonSerialize()
    {
        $ds18B20hour = 0;
        if ($this->getTemperatureDS18B20(self::LAST_HOUR) !== null) {
            $ds18B20hour = $this->getTemperatureDS18B20() - $this->getTemperatureDS18B20(self::LAST_HOUR);
        }
        $bme280hour = 0;
        if ($this->getTemperatureBME280(self::LAST_HOUR) !== null) {
            $bme280hour = $this->getTemperatureBME280() - $this->getTemperatureBME280(self::LAST_HOUR);
        }
        $humidity1hour = 0;
        if ($this->getHumidity(self::LAST_HOUR) !== null) {
            $humidity1hour = $this->getHumidity() - $this->getHumidity(self::LAST_HOUR);
        }

        return [
            'temperature_DS18B20' => $this->getTemperatureDS18B20(),
            'temperature_max_DS18B20' => $this->getTemperatureDS18B20(self::MAX),
            'temperature_min_DS18B20' => $this->getTemperatureDS18B20(self::MIN),
            'temperature_1hour_DS18B20' => $ds18B20hour,
            'temperature_BME280' => $this->getTemperatureBME280(),
            'temperature_max_BME280' => $this->getTemperatureBME280(self::MAX),
            'temperature_min_BME280' => $this->getTemperatureBME280(self::MIN),
            'temperature_1hour_BME280' => $bme280hour,
            'humidity' => $this->getHumidity(),
            'humidity_max' => $this->getHumidity(self::MAX),
            'humidity_min' => $this->getHumidity(self::MIN),
            'humidity_1hour' => $humidity1hour,
            'pressure' => $this->getPressureMmHg(),
            'pressure_each_hour' => $this->getPressureMmHg(self::EACH_HOUR),
            'lux' => $this->getLux(),
            'vcc' => $this->getVcc(),
            'lastUpdate' => $this->getLastUpdate()
        ];
    }
}