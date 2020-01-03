<?php


namespace App\Radio;


class LoraCommand extends RadioAbstract
{

    protected function getDirection()
    {
        return self::DIRECTION_OUT;
    }

    protected function getModuleType()
    {
        return self::TYPE_LORA;
    }
}