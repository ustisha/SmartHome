<?php


namespace App\Command;


use App\Entity\User;
use Doctrine\ORM\EntityManagerInterface;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Security\Core\Encoder\EncoderFactoryInterface;

class CreateUser extends Command
{
    // the name of the command (the part after "bin/console")
    protected static $defaultName = 'app:create-user';
    /**
     * @var EncoderFactoryInterface
     */
    private $factory;
    /**
     * @var EntityManagerInterface
     */
    private $em;

    public function __construct(EncoderFactoryInterface $factory, EntityManagerInterface $em, $name = null)
    {
        parent::__construct($name);
        $this->factory = $factory;
        $this->em = $em;
    }

    protected function configure()
    {
        $this->addArgument('username', InputArgument::REQUIRED, 'Username');
        $this->addArgument('password', InputArgument::REQUIRED, 'User password');
    }

    /**
     * @param InputInterface  $input
     * @param OutputInterface $output
     *
     * @return int|void
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $user = new User();
        $encoder = $this->factory->getEncoder($user);
        $pass = $encoder->encodePassword($input->getArgument('password'), null);
        $user->setEmail($input->getArgument('username'));
        $user->setPassword($pass);
        $this->em->persist($user);
        $this->em->flush();

        return 0;
    }
}