<?php

namespace App\Controller;

use App\Model\LogTable;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\Routing\Annotation\Route;

class LogController extends AbstractController
{
    /**
     * @Route("/log/table/{page}", name="log_table")
     * @param int      $page
     * @param LogTable $logTable
     *
     * @return JsonResponse
     */
    public function table(int $page, LogTable $logTable)
    {
        $logTable->setPage($page);
        return $this->json($logTable);
    }
}
