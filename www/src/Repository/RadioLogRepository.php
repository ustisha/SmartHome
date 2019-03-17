<?php

namespace App\Repository;

use App\Entity\RadioLog;
use App\Model\Net;
use App\Radio\RadioAbstract;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
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
     * @param int $sender
     *
     * @return RadioLog[]
     */
    public function loadLastLog(int $sender)
    {
        $qbGroup = $this->createQueryBuilder('rl');
        $qbGroup->select('MAX(rl.id) ')
            ->andWhere('rl.sender = :sender')
            ->andWhere('rl.direction = :direction')
            ->andWhere($qbGroup->expr()->in('rl.sender_port', Net::ports()))
            ->groupBy('rl.sender, rl.sender_port, rl.command');

        $qb = $this->createQueryBuilder('l');
        $qb->setParameter('sender', $sender)
            ->setParameter('direction', RadioAbstract::DIRECTION_IN);
        $qb->andWhere($qb->expr()->in('l.id', $qbGroup->getDQL()));

        return $qb->getQuery()->execute();
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
