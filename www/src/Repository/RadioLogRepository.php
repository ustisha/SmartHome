<?php

namespace App\Repository;

use App\Entity\RadioLog;
use App\Model\Net;
use App\Radio\RadioAbstract;
use App\Repository\RadioLog\MaxValue;
use DateTime;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Doctrine\ORM\Internal\HydrationCompleteHandler;
use Symfony\Bridge\Doctrine\RegistryInterface;

/**
 * @method RadioLog|null find($id, $lockMode = null, $lockVersion = null)
 * @method RadioLog|null findOneBy(array $criteria, array $orderBy = null)
 * @method RadioLog[]    findAll()
 * @method RadioLog[]    findBy(array $criteria, array $orderBy = null, $limit = null, $offset = null)
 */
class RadioLogRepository extends ServiceEntityRepository
{
    public function __construct(RegistryInterface $registry)
    {
        parent::__construct($registry, RadioLog::class);
    }

    /**
     * @param int   $sender
     *
     * @param array $commands
     *
     * @return RadioLog[]
     */
    public function loadLastLog(int $sender, array $commands = [])
    {
        $qb = $this->createQueryBuilder('l');
        $qbGroup = $this->createQueryBuilder('rl');
        $qbGroup->select('MAX(rl.id) ')
            ->andWhere('rl.sender = :sender')
            ->andWhere('rl.direction = :direction')
            ->andWhere($qbGroup->expr()->in('rl.sender_port', Net::ports()))
            ->groupBy('rl.sender, rl.sender_port, rl.command');
        if (count($commands)) {
            $qbGroup->andWhere($qb->expr()->in('rl.command', $commands));
        }

        $qb->setParameter('sender', $sender)
            ->setParameter('direction', RadioAbstract::DIRECTION_IN);
        $qb->andWhere($qb->expr()->in('l.id', $qbGroup->getDQL()));

        return $qb->getQuery()->execute();
    }


    /**
     * @param int      $sender
     * @param DateTime $from
     * @param DateTime $to
     * @param array    $commands
     *
     * @return MaxValue[]
     */
    public function loadMaxValues(int $sender, DateTime $from, DateTime $to, array $commands = []): array
    {
        $qb = $this->createQueryBuilder('rl');
        $qb->select('rl.sender_port', 'rl.command', $qb->expr()->max('rl.data') . 'AS data')
            ->andWhere('rl.sender = :sender')
            ->andWhere('rl.direction = :direction')
            ->andWhere($qb->expr()->in('rl.sender_port', Net::ports()))
            ->andWhere('rl.date > :from')
            ->andWhere('rl.date < :to')
            ->groupBy('rl.sender, rl.sender_port, rl.command');

        if (count($commands)) {
            $qb->andWhere($qb->expr()->in('rl.command', $commands));
        }

        $qb->setParameter('sender', $sender)
            ->setParameter('direction', RadioAbstract::DIRECTION_IN)
            ->setParameter('from', $from)
            ->setParameter('to', $to);

        return $qb->getQuery()
            ->getResult();
    }

    // /**
    //  * @return RadioLog[] Returns an array of RadioLog objects
    //  */
    /*
    public function findByExampleField($value)
    {
        return $this->createQueryBuilder('r')
            ->andWhere('r.exampleField = :val')
            ->setParameter('val', $value)
            ->orderBy('r.id', 'ASC')
            ->setMaxResults(10)
            ->getQuery()
            ->getResult()
        ;
    }
    */

    /*
    public function findOneBySomeField($value): ?RadioLog
    {
        return $this->createQueryBuilder('r')
            ->andWhere('r.exampleField = :val')
            ->setParameter('val', $value)
            ->getQuery()
            ->getOneOrNullResult()
        ;
    }
    */
}
