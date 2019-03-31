<?php

namespace App\Controller;

use App\Model\OutsideTemp;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\Routing\Annotation\Route;

class OutsideTempController extends AbstractController
{
    /**
     * @Route("/outside/temp", name="outside_temp")
     * @param OutsideTemp $outsideTemp
     *
     * @return \Symfony\Component\HttpFoundation\JsonResponse
     */
    public function index(OutsideTemp $outsideTemp)
    {
        return $this->json($outsideTemp);
    }
}
