<?php


namespace App\Repository\RadioLog;


class AggregateValue
{

    protected $sender_port;

    protected $command;

    protected $data;

    public function __construct($senderPort, $command, $data)
    {
        $this->sender_port = $senderPort;
        $this->command = $command;
        $this->data = $data;
    }

    /**
     * @return mixed
     */
    public function getSenderPort()
    {
        return $this->sender_port;
    }

    /**
     * @return mixed
     */
    public function getCommand()
    {
        return $this->command;
    }

    /**
     * @return mixed
     */
    public function getData()
    {
        return $this->data;
    }
}