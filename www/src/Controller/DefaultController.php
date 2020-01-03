<?php


namespace App\Controller;

use Symfony\Component\Routing\Annotation\Route;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

class DefaultController extends AbstractController
{

    /**
     *
     */
    public function index()
    {
        return $this->render(
            'base.html.twig'
        );
    }
}