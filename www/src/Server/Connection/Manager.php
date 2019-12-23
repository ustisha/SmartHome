<?php


namespace App\Server\Connection;


use App\Server\Connection;

class Manager
{

    /**
     * @var Connection[]
     */
    private $connections = [];

    /**
     * @param Connection $connection
     */
    public function add(Connection $connection)
    {
        $this->connections[$connection->getConnectionId()] = $connection;
    }

    /**
     * @param string $component
     *
     * @return Connection[]
     */
    public function getByComponent($component)
    {
        $found = [];
        foreach ($this->connections as $connection) {
            if ($connection->getComponent() == $component) {
                $found[] = $connection;
            }
        }
        return $found;
    }

    /**
     * @param string $component
     * @param array  $data
     */
    public function sendByComponent($component, array $data)
    {
        foreach ($this->getByComponent($component) as $connection) {
            $connection->send($data);
        }
    }

    /**
     * @param $connectionId
     */
    public function removeById($connectionId)
    {
        if (isset($this->connections[$connectionId])) {
            unset($this->connections[$connectionId]);
        }
    }
}