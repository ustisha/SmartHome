<?php


namespace App\Radio;


class Nrf24Command extends RadioAbstract
{

    protected function getDirection()
    {
        return self::DIRECTION_OUT;
    }

    protected function getModuleType()
    {
        return self::TYPE_NRF24;
    }
}