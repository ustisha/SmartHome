<?php


namespace App\Controller;

use App\Radio\LoraRequest;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class LoraController
{

    /**
     * @Route("/lucky/number")
     *
     * @param LoraRequest $loraRequest
     *
     * @return Response
     */
    public function receive(LoraRequest $loraRequest)
    {
        $loraRequest->saveRequest();
        return new Response('OK');
    }

}