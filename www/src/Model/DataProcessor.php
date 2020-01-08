<?php


namespace App\Model;


use App\Entity\RadioLog;
use DateInterval;
use DateTime;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NonUniqueResultException;
use Exception;

class DataProcessor implements DataProcessorInterface
{
    protected $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    /**
     * @param RadioLog $radioLog
     *
     * @return array
     * @throws Exception
     */
    public function getData(RadioLog $radioLog)
    {
        $cmdMethod = ucfirst(str_replace('_', '', ucwords(Net::getCommandName($radioLog->getCommand()), '_')));
        $processMethod = "process$cmdMethod";
        if (method_exists($this, $processMethod)) {
            $data = $this->$processMethod($radioLog);
        } elseif (in_array($radioLog->getCommand(), Net::getFloatCommands())) {
            $data = $this->processFloat($radioLog);
        } else {
            $data = $this->processDefault($radioLog);
        }
        try {
            $lastUpdate = $this->entityManager->getRepository(RadioLog::class)->getLastUpdate(
                $radioLog->getSender()
            );
            $data['lastUpdate'] = $lastUpdate->format('c');
        } catch (NonUniqueResultException $e) {

        } catch (Exception $e) {
            throw $e;
        }
        return $data;
    }

    protected function getKey(RadioLog $radioLog)
    {
        $a = [
            Net::getPortName($radioLog->getSenderPort()),
            Net::getCommandName($radioLog->getCommand())
        ];
        return implode('_', $a);
    }

    /**
     * @param RadioLog $radioLog
     *
     * @return mixed
     */
    protected function processDefault(RadioLog $radioLog)
    {
        return [
            $this->getKey($radioLog) => $radioLog->getData()
        ];
    }

    /**
     * @param RadioLog $radioLog
     *
     * @return array
     */
    protected function processFloat(RadioLog $radioLog)
    {
        $key = $this->getKey($radioLog);
        return [$key => $radioLog->getData() / 100];
    }

    /**
     * @param RadioLog $radioLog
     *
     * @return array
     * @throws Exception
     */
    protected function processPressure(RadioLog $radioLog)
    {
        $key = $this->getKey($radioLog);
        $f = function ($value) {
            return round($value / 13332.2387415, 2);
        };
        return array_merge(
            [$key => $f($radioLog->getData())],
            $this->commonHourProcess($radioLog, $f)
        );
    }

    /**
     * @param RadioLog $radioLog
     *
     * @return array
     * @throws Exception
     */
    protected function processTemperature(RadioLog $radioLog)
    {
        $key = $this->getKey($radioLog);
        $f = function ($value) {
            return $value / 100;
        };
        $value = $f($radioLog->getData());
        return array_merge(
            [$key => $value],
            $this->commonStatProcess($radioLog, $f, $value)
        );
    }

    /**
     * @param RadioLog $radioLog
     *
     * @return array
     * @throws Exception
     */
    protected function processHumidity(RadioLog $radioLog)
    {
        $key = $this->getKey($radioLog);
        $f = function ($value) {
            return $value / 100;
        };
        $value = $f($radioLog->getData());
        return array_merge(
            [$key => $value],
            $this->commonStatProcess($radioLog, $f, $value)
        );
    }

    /**
     * @param RadioLog      $radioLog
     * @param callable|null $dataCalc
     * @param array         $hours
     *
     * @return array
     * @throws Exception
     */
    protected function commonHourProcess(RadioLog $radioLog, callable $dataCalc = null, $hours = [0, 1, 2, 3, 6, 12])
    {
        if ($dataCalc === null) {
            $dataCalc = function ($value) {
                return $value;
            };
        }
        $data = [];
        foreach ($hours as $i) {
            $from = (new DateTime("-$i hour"));
            $toHour = $i - 1;
            $to = (new DateTime("-$toHour hour -55 min"));
            $rows = $this->entityManager->getRepository(RadioLog::class)->loadAvgValues(
                $radioLog->getSender(),
                $from,
                $to,
                [$radioLog->getCommand()]
            );
            foreach ($rows as $row) {
                $data["{$i}h"] = $dataCalc($row->getData());
            }
        }
        $data = array_reverse($data, true);
        return [
            $this->getKey($radioLog) . '_each_hour' => $data
        ];
    }

    /**
     * @param RadioLog        $radioLog
     * @param callable|null   $dataCalc
     * @param int|float| null $currentValue
     *
     * @return array
     * @throws Exception
     */
    protected function commonStatProcess(RadioLog $radioLog, callable $dataCalc = null, $currentValue = null)
    {
        if ($dataCalc === null) {
            $dataCalc = function ($value) {
                return $value;
            };
        }

        $data = [];
        $key = $this->getKey($radioLog);
        $from = (new DateTime())->setTime(9, 0, 0);
        $to = (new DateTime())->setTime(23, 59, 59);
        if (new DateTime() < $from) {
            $from->sub((new DateInterval('P1D')));
            $to->sub((new DateInterval('P1D')));
        }
        $rows = $this->entityManager->getRepository(RadioLog::class)->loadMaxValues(
            $radioLog->getSender(),
            $from,
            $to,
            [$radioLog->getCommand()],
            $radioLog->getSenderPort()
        );
        foreach ($rows as $row) {
            if ($row->getCommand() == $radioLog->getCommand()) {
                $data[$key . '_max'] = $dataCalc($row->getData());
            }
        }

        $from = (new DateTime())->setTime(0, 0, 0);
        $to = (new DateTime())->setTime(8, 59, 59);
        if (new DateTime() < $from) {
            $from->sub((new DateInterval('P1D')));
            $to->sub((new DateInterval('P1D')));
        }
        $rows = $this->entityManager->getRepository(RadioLog::class)->loadMinValues(
            $radioLog->getSender(),
            $from,
            $to,
            [$radioLog->getCommand()],
            $radioLog->getSenderPort()
        );
        foreach ($rows as $row) {
            if ($row->getCommand() == $radioLog->getCommand()) {
                $data[$key . '_min'] = $dataCalc($row->getData());
            }
        }

        $from = (new DateTime('-1 hour'));
        $to = (new DateTime('-55 min'));
        $rows = $this->entityManager->getRepository(RadioLog::class)->loadAvgValues(
            $radioLog->getSender(),
            $from,
            $to,
            [$radioLog->getCommand()],
            $radioLog->getSenderPort()
        );
        foreach ($rows as $row) {
            if ($row->getCommand() == $radioLog->getCommand()) {
                if ($currentValue !== null) {
                    $data[$key . '_hour'] = $currentValue - $dataCalc($row->getData());
                } else {
                    $data[$key . '_hour'] = $dataCalc($row->getData());
                }
            }
        }

        return $data;
    }
}