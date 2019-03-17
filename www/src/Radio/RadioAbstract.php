<?php


namespace App\Radio;

use App\Entity\RadioLog;
use Doctrine\ORM\EntityManagerInterface;
use Symfony\Component\HttpFoundation\RequestStack;

abstract class RadioAbstract
{
    const TYPE_LORA = 1;
    const TYPE_NRF24 = 2;

    const DIRECTION_IN = 1;
    const DIRECTION_OUR = 2;

    private $log;

    private $entityManager;

    public function __construct(RequestStack $requestStack, EntityManagerInterface $entityManager)
    {
        $request = $requestStack->getCurrentRequest();

        $this->log = new RadioLog();
        $this->log->setDate(new \DateTime());
        $this->log->setType($this->getModuleType());
        $this->log->setDirection(self::DIRECTION_IN);
        $this->log->setSender($request->get('s'));
        $this->log->setSenderPort($request->get('sp'));
        $this->log->setReceiver($request->get('r'));
        $this->log->setReceiverPort($request->get('rp'));
        $this->log->setCommand($request->get('cmd'));
        $this->log->setData($request->get('data'));
        $this->entityManager = $entityManager;
    }

    abstract protected function getModuleType();


    public function saveRequest()
    {
        $this->entityManager->persist($this->log);
        $this->entityManager->flush();
        return true;
    }
}