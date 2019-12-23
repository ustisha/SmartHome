<?php


namespace App\Radio;

class LoraRequest extends RadioAbstract
{

    protected function getModuleType()
    {
        return self::TYPE_LORA;
    }

    protected function getDirection()
    {
        return self::DIRECTION_IN;
    }
}