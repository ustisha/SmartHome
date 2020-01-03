<?php

namespace App;

require_once __DIR__ . '/../vendor/autoload.php';

use Workerman\Connection\ConnectionInterface;
use Workerman\Worker;

// create a local tcp-server. it will receive messages from your site code (for example from send.php)
$tcp = new Worker("tcp://127.0.0.1:8111");
// create a handler that will be called when a local tcp-socket receives a message (for example from send.php)
$tcp->onMessage = function ($connection, $data) use ($tcp) {
    Worker::safeEcho("Message, connection id: {$connection->id}, data: " . json_encode($data) . PHP_EOL);
    // forward message to all other process (you have 4 ws-servers)
    foreach ($tcp->connections as $id => $webconnection) {
        Worker::safeEcho("Connections data send, id: $id" . PHP_EOL);
        if ($connection->id != $id) {
            /** @var ConnectionInterface $webconnection */
            $webconnection->send($data);
        }
    }
};
// Run worker
Worker::runAll();
