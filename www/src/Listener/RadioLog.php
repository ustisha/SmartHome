<?php


namespace App\Listener;


use App\Entity\RadioLog as RadioLogEntity;
use App\Model\DataProcessorInterface;
use App\Model\Net;
use App\Radio\RadioAbstract;
use Doctrine\Persistence\Event\LifecycleEventArgs;
use Symfony\Component\HttpClient\HttpClient;
use Symfony\Component\HttpClient\NativeHttpClient;
use Symfony\Contracts\HttpClient\Exception\TransportExceptionInterface;

class RadioLog
{
    /**
     * @var DataProcessorInterface
     */
    protected $dataProcessor;

    public function __construct(DataProcessorInterface $dataProcessor)
    {
        $this->dataProcessor = $dataProcessor;
    }

    public function postPersist(RadioLogEntity $radioLog, LifecycleEventArgs $event)
    {
        if ($radioLog->getDirection() == RadioAbstract::DIRECTION_IN) {
            $socket = 'tcp://127.0.0.1:8111';
            $instance = stream_socket_client($socket);
            if ($instance !== false) {
                fwrite(
                    $instance,
                    json_encode(
                        [
                            'component' => Net::getComponentName($radioLog->getSender()),
                            'data' => $this->dataProcessor->getData($radioLog)
                        ]
                    ) . "\n"
                );
            }
        } elseif ($radioLog->getDirection() == RadioAbstract::DIRECTION_OUT) {
            $query = http_build_query([
                's' => $radioLog->getSender(),
                'sp' => $radioLog->getSenderPort(),
                'r' => $radioLog->getReceiver(),
                'rp' => $radioLog->getReceiverPort(),
                'cmd' => $radioLog->getCommand(),
                'data' => $radioLog->getData()
            ]);

            /** @var NativeHttpClient $httpClient */
            $httpClient = HttpClient::create();
            try {
                // @todo Log this problem on fail.
                $response = $httpClient->request('GET', "http://192.168.10.77?$query");
                $statusCode = $response->getStatusCode();
            } catch (TransportExceptionInterface $e) {

            }
        }
    }
}