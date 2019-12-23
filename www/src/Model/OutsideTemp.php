<?php


namespace App\Model;


use App\Entity\RadioLog;
use DateTime;
use Doctrine\ORM\EntityManagerInterface;

class OutsideTemp implements \JsonSerializable
{

    /**
     * @var EntityManagerInterface
     */
    private $entityManager;

    /**
     * @var DataProcessorInterface
     */
    private $dataProcessor;

    /**
     * @var DateTime
     */
    protected $lastUpdate;

    public function __construct(EntityManagerInterface $entityManager, DataProcessorInterface $dataProcessor)
    {
        $this->entityManager = $entityManager;
        $this->dataProcessor = $dataProcessor;
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
        $data = [];
        $this->lastUpdate = new DateTime();
        $commands = [Net::CMD_TEMPERATURE, Net::CMD_HUMIDITY, Net::CMD_PRESSURE, Net::CMD_LIGHT, Net::CMD_VCC];
        $rows = $this->entityManager->getRepository(RadioLog::class)->senderLog(Net::OUTSIDE_TEMP, $commands);
        foreach ($rows as $outside) {
            $data = array_merge($data, $this->dataProcessor->getData($outside));
            if ($outside->getDate() < $this->getLastUpdate()) {
                $this->lastUpdate = $outside->getDate();
            }
        }
        $data['lastUpdate'] = $this->getLastUpdate();
        return $data;
    }
}