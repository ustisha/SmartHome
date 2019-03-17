<?php


namespace App\Model;


use App\Entity\RadioLog;
use Doctrine\ORM\EntityManagerInterface;

class OutsideTemp
{
    protected $temperature = [];

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
        $data = $entityManager->getRepository(RadioLog::class)->loadLastLog(Net::OUTSIDE_TEMP);
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

}