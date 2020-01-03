<?php


namespace App\Controller;

use App\Radio\LoraRequest;
use App\Radio\Nrf24Request;
use Psr\Log\LoggerInterface;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class ReceiveController
{

    /**
     * @var LoggerInterface
     */
    private $logger;

    public function __construct(LoggerInterface $logger)
    {
        $this->logger = $logger;
    }

    /**
     * @Route("/receive/ready", methods={"GET"}, name="receive_ready")
     */
    public function ready()
    {
        $this->logger->info("Gateway ready to receive");
    }

    /**
     * @Route("/receive/ping", methods={"GET"}, name="receive_ping")
     */
    public function ping()
    {
        $this->logger->debug("Gateway ping");
    }

    /**
     * @Route("/receive/lora", methods={"GET"}, name="receive_lora")
     *
     * @param LoraRequest $loraRequest
     *
     * @return Response
     */
    public function lora(LoraRequest $loraRequest)
    {
        $loraRequest->saveRequest();
        $this->logger->debug("Receive LoRa request");
        return new Response('OK');
    }

    /**
     * @Route("/receive/nrf24", methods={"GET"}, name="receive_nrf24")
     *
     * @param Nrf24Request $nrf24Request
     *
     * @return Response
     */
    public function nrf24(Nrf24Request $nrf24Request)
    {
        $nrf24Request->saveRequest();
        $this->logger->debug("Receive NRF24 request");
        return new Response('OK');
    }

}