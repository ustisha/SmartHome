<?php

namespace App\Controller;

use App\Radio\WwwRequest;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

class GreenhouseController extends AbstractController
{
    /**
     * @Route("/greenhouse/data", name="greenhouse_data")
     *
     * @return \Symfony\Component\HttpFoundation\JsonResponse
     */
    public function data()
    {
        return $this->json([]);
    }

    /**
     * @Route("/greenhouse/command", methods={"POST"}, name="greenhouse_command")
     * @param WwwRequest $request
     *
     * @return \Symfony\Component\HttpFoundation\JsonResponse
     */
    public function command(WwwRequest $request)
    {
        $success = $request->saveRequest();
        return $this->json(
            [
                'success' => $success
            ]
        );
    }
}
