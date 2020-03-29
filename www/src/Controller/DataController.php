<?php

namespace App\Controller;

use App\Model\OutsideTemp;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\Routing\Annotation\Route;

class DataController extends AbstractController
{
    /**
     * @Route("/data/outside", methods={"GET"}, name="data_outside")
     *
     * @param OutsideTemp $outsideTemp
     *
     * @return JsonResponse
     */
    public function outside(OutsideTemp $outsideTemp)
    {
        return $this->json($outsideTemp);
    }

    /**
     * @Route("/data/coldchamber", methods={"GET"}, name="data_coldchamber")
     *
     * @return JsonResponse
     */
    public function coldchamber()
    {
        return $this->json([]);
    }

    /**
     * @Route("/data/bathroom", methods={"GET"}, name="data_bathroom")
     *
     * @return JsonResponse
     */
    public function bathroom()
    {
        return $this->json([]);
    }
}
