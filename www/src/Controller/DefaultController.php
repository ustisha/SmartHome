<?php


namespace App\Controller;


use App\Model\OutsideTemp;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

class DefaultController extends AbstractController
{

    public function index(OutsideTemp $outsideTemp)
    {
        return $this->forward('\App\Controller\MeteoController::index');
//        return $this->render(
//            'base.html.twig',
//            [
//                'outsideTemp' => $outsideTemp,
//            ]
//        );
    }
}