<?php


namespace App\Radio;

class Nrf24Request extends RadioAbstract
{

    protected function getModuleType()
    {
        return self::TYPE_NRF24;
    }

    protected function getDirection()
    {
        return self::DIRECTION_IN;
    }
}