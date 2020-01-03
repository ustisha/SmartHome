<?php


namespace App\Controller;


use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\Security\Core\Security;

class AuthController extends AbstractController
{

    /**
     * @var Security
     */
    private $security;

    public function __construct(Security $security)
    {
        $this->security = $security;
    }

    public function current()
    {
        $data = [];
        if ($user = $this->security->getUser()) {
            $data['username'] = $user->getUsername();
            $data['roles'] = $user->getRoles();
        }
        return $this->json($data);
    }

    /**
     *
     */
    public function login()
    {
        $user = $this->security->getUser();
        return $this->json(
            [
                'username' => $user->getUsername(),
                'roles' => $user->getRoles(),
            ]
        );
    }

}