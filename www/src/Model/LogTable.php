<?php


namespace App\Model;


use App\Entity\RadioLog;
use Doctrine\ORM\EntityManagerInterface;

class LogTable implements \JsonSerializable
{

    /**
     * @var EntityManagerInterface
     */
    private $entityManager;

    /**
     * @var int
     */
    protected $page;

    /**
     * LogTable constructor.
     *
     * @param EntityManagerInterface $entityManager
     */
    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
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
        $log = $this->entityManager->getRepository(RadioLog::class)->commandsLog(
            [
                Net::CMD_INFO
            ],
            $this->page
        );

        return [
            'length' => $log->count(),
            'page' => $log->getCurrentPage(),
            'rows' => $log->getIterator()
        ];
    }

    /**
     * @param int $page
     *
     * @return LogTable
     */
    public function setPage(int $page): LogTable
    {
        $this->page = $page;
        return $this;
    }
}