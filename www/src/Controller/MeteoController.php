<?php


namespace App\Controller;


use App\Model\OutsideTemp;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

class MeteoController extends AbstractController
{

    public function index()
    {
        return $this->render(
            'meteo.html.twig'
        );
    }
}