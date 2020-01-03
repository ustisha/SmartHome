<?php


namespace App\Model;


use App\Entity\RadioLog;

interface DataProcessorInterface
{

    /**
     * @param RadioLog $radioLog
     *
     * @return array
     */
    public function getData(RadioLog $radioLog);

}