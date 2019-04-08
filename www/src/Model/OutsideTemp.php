<?php


namespace App\Model;


use App\Entity\RadioLog;
use Doctrine\ORM\EntityManagerInterface;

class OutsideTemp implements \JsonSerializable
{
    protected $temperature = [];

    protected $temperatureMax = [];

    protected $humidity;

    /**
     * Pressure in Pa
     *
     * @var int|null
     */
    protected $pressure;

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
        $data = $entityManager->getRepository(RadioLog::class)->loadLastLog(Net::OUTSIDE_TEMP, $commands);
        $this->lastUpdate = new \DateTime();
        foreach ($data as $outside) {
            if ($outside->getCommand() == Net::CMD_TEMPERATURE) {
                $this->temperature[$outside->getSenderPort()] = $outside->getData() / 100;
            } elseif ($outside->getCommand() == Net::CMD_HUMIDITY) {
                $this->humidity = $outside->getData() / 100;
            } elseif ($outside->getCommand() == Net::CMD_PRESSURE) {
                $this->pressure = $outside->getData();
            } elseif ($outside->getCommand() == Net::CMD_LIGHT) {
                $this->lux = $outside->getData();
            } elseif ($outside->getCommand() == Net::CMD_VCC) {
                $this->vcc = $outside->getData() / 100;
            }
            if ($this->lastUpdate > $outside->getDate()) {
                $this->lastUpdate = $outside->getDate();
            }
        }

        $from = (new \DateTime('-4 day'))->setTime(12, 0, 0);
        $to = (new \DateTime('-4 day'))->setTime(23, 59, 59);
        $data = $entityManager->getRepository(RadioLog::class)->loadMaxValues(Net::OUTSIDE_TEMP, $from, $to, [Net::CMD_TEMPERATURE]);
        var_dump($data);
        foreach ($data as $row) {
            $this->temperatureMax[$row['sender_port']] = $row['data'] / 100;
        }

    }

    /**
     * @param int $sensor
     *
     * @return float|null
     */
    private function getTemp(int $sensor)
    {
        if (isset($this->temperature[$sensor])) {
            return $this->temperature[$sensor];
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
     * @return float|null
     */
    public function getTemperatureDS18B20()
    {
        return $this->getTemp(Net::OUTSIDE_TEMP_18B20);
    }

    /**
     * @return float|null
     */
    public function getTemperatureBME280()
    {
        return $this->getTemp(Net::OUTSIDE_TEMP_BME280);
    }

    /**
     * @return float|int
     */
    public function getHumidity()
    {
        return $this->humidity;
    }

    public function getPressureMmHg()
    {
        return round($this->pressure / 13332.2387415, 2);
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
        return [
            'temperature_DS18B20' => $this->getTemperatureDS18B20(),
            'temperature_BME280' => $this->getTemperatureBME280(),
            'humidity' => $this->getHumidity(),
            'pressure' => $this->getPressureMmHg(),
            'lux' => $this->getLux(),
            'vcc' => $this->getVcc(),
            'lastUpdate' => $this->getLastUpdate()
        ];
    }
}