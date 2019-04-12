<?php


namespace App\Repository\RadioLog\AggregateValue;


use App\Repository\RadioLog\AggregateValue;
use Doctrine\ORM\Internal\Hydration\AbstractHydrator;
use PDO;

class Hydrator extends AbstractHydrator
{

    /**
     * Hydrates all rows from the current statement instance at once.
     *
     * @return AggregateValue[]
     */
    protected function hydrateAllData()
    {
        $rows = [];
        foreach ($this->_stmt->fetchAll(PDO::FETCH_ASSOC) as $row) {
            $row = $this->gatherScalarRowData($row);
            $rows[] = new AggregateValue(
                (int)$row['sender_port'],
                (int)$row['command'],
                (int)$row['data']
            );
        }
        return $rows;
    }
}