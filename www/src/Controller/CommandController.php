<?php

namespace App\Controller;

use App\Radio\LoraCommand;
use App\Radio\Nrf24Command;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class CommandController extends AbstractController
{
    /**
     * @Route("/command/lora", methods={"POST"}, name="command_lora")
     * @param LoraCommand $request
     *
     * @return JsonResponse
     */
    public function lora(LoraCommand $request)
    {
        $success = $request->saveRequest();
        return $this->json(
            [
                'success' => $success
            ]
        );
    }

    /**
     * @Route("/command/nrf24", methods={"POST"}, name="command_nrf24")
     * @param Nrf24Command $request
     *
     * @return JsonResponse
     */
    public function nrf24(Nrf24Command $request)
    {
        $success = $request->saveRequest();
        return $this->json(
            [
                'success' => $success
            ]
        );
    }
}
