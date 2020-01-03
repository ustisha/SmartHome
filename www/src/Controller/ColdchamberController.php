<?php

namespace App\Controller;

use App\Radio\WwwRequest;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class ColdchamberController extends AbstractController
{
    /**
     * @Route("/coldchamber/data", name="coldchamber_data")
     *
     * @return \Symfony\Component\HttpFoundation\JsonResponse
     */
    public function data()
    {
        return $this->json([]);
    }

    /**
     * @Route("/coldchamber/command", methods={"POST"}, name="coldchamber_command")
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
