<?php


namespace App\Server;


use Workerman\Connection\ConnectionInterface;

class Connection
{

    /**
     * @var ConnectionInterface
     */
    protected $connection;

    /**
     * @var string
     */
    protected $component;

    public function __construct(ConnectionInterface $connection, $component = null)
    {
        $this->connection = $connection;
        $this->component = $component;
    }

    /**
     * @return int
     */
    public function getConnectionId() {
        return $this->connection->id;
    }

    /**
     * @return string
     */
    public function getComponent(): string
    {
        return $this->component;
    }

    /**
     * @param array $data
     *
     * @return bool|void
     */
    public function send(array $data) {
        return $this->connection->send(json_encode($data));
    }

}