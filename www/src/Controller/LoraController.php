<?php


namespace App\Controller;

use Psr\Log\LoggerInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class LoraController
{

    /**
     * @Route("/lucky/number")
     *
     * @param Request         $request
     * @param LoggerInterface $logger
     *
     * @return Response
     */
    public function receive(Request $request, LoggerInterface $logger)
    {
        $sender = $request->get('s');
        $senderPort = $request->get('sp');
        $receiver = $request->get('r');
        $receiverPort = $request->get('rp');
        $command = $request->get('cmd');
        $data = $request->get('data');


        $logger->info(
            "Sender: $sender, Sender port: $senderPort, Receiver: $receiver, Receiver port: $receiverPort, Command: $command, Data: $data"
        );

        return new Response('OK');
    }

}