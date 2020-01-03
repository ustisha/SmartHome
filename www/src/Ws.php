<?php

namespace App;

require_once __DIR__ . '/../vendor/autoload.php';

use App\Server\Connection;
use App\Server\Connection\Manager;
use Workerman\Connection\ConnectionInterface;
use Workerman\Worker;
use Workerman\Connection\AsyncTcpConnection;

// create a ws-server. all your users will connect to it
$ws = new Worker("websocket://0.0.0.0:8081");
$ws->count = 1;// it will create 4 process

// storage of user-connection link
$cm = new Manager();

$ws->onConnect = function ($connection) use ($cm) {
    /**
     * @param ConnectionInterface $connection
     */
    $connection->onWebSocketConnect = function ($connection) use ($cm) {
        // put get-parameter into $users collection when a new user is connected
        // you can set any parameter on site page. for example client.html: ws = new WebSocket("ws://127.0.0.1:8000/?user=tester01");
        $component = htmlspecialchars($_GET['component']);
        Worker::safeEcho("Connect component: '$component', connection id: {$connection->id}" . PHP_EOL);
        $c = new Connection($connection, $component);
        $cm->add($c);
    };
};

$ws->onClose = function ($connection) use (&$cm) {
    Worker::safeEcho("Connection close, connection id: {$connection->id}" . PHP_EOL);
    $cm->removeById($connection->id);
};

// it will start once for each of the 4 ws-servers when you start server.php:
$ws->onWorkerStart = function () use (&$cm) {
    //each ws-server connects to the local tcp-server
    $connection = new AsyncTcpConnection("tcp://127.0.0.1:8111");
    $connection->onMessage = function ($connection, $data) use (&$cm) {
        // you have to use json_decode for $data because send.php uses json_encode
        Worker::safeEcho("Message, connection id: {$connection->id}, data: $data" . PHP_EOL);
        $data = json_decode($data, JSON_OBJECT_AS_ARRAY); // but you can use another protocol for send data send.php to local tcp-server
        $cm->sendByComponent($data['component'], $data['data']);
    };
    $connection->onConnect = function ($connection) {
        Worker::safeEcho("Acync connection, connection id: {$connection->id}" . PHP_EOL);
    };
    $connection->connect();
};
// Run worker
Worker::runAll();