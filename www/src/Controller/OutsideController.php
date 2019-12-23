<?php

namespace App\Controller;

use App\Model\OutsideTemp;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\Routing\Annotation\Route;

class OutsideController extends AbstractController
{
    /**
     * @Route("/outside/data", name="outside_data")
     * @param OutsideTemp $outsideTemp
     *
     * @return \Symfony\Component\HttpFoundation\JsonResponse
     */
    public function data(OutsideTemp $outsideTemp)
    {
        return $this->json($outsideTemp);
    }
}
