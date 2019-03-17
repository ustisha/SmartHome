<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity(repositoryClass="App\Repository\RadioLogRepository")
 */
class RadioLog
{
    /**
     * @ORM\Id()
     * @ORM\GeneratedValue()
     * @ORM\Column(type="integer")
     */
    private $id;

    /**
     * @ORM\Column(type="datetime")
     */
    private $date;

    /**
     * @ORM\Column(type="smallint")
     */
    private $type;

    /**
     * @ORM\Column(type="smallint")
     */
    private $direction;

    /**
     * @ORM\Column(type="integer")
     */
    private $sender;

    /**
     * @ORM\Column(type="integer")
     */
    private $sender_port;

    /**
     * @ORM\Column(type="integer")
     */
    private $receiver;

    /**
     * @ORM\Column(type="integer")
     */
    private $receiver_port;

    /**
     * @ORM\Column(type="smallint")
     */
    private $command;

    /**
     * @ORM\Column(type="bigint")
     */
    private $data;

    /**
     * @ORM\Column(type="integer")
     */
    private $rssi;

    /**
     * @ORM\Column(type="float")
     */
    private $snr;

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getDate(): ?\DateTimeInterface
    {
        return $this->date;
    }

    public function setDate(\DateTimeInterface $date): self
    {
        $this->date = $date;

        return $this;
    }

    public function getType(): ?int
    {
        return $this->type;
    }

    public function setType(int $type): self
    {
        $this->type = $type;

        return $this;
    }

    public function getDirection(): ?int
    {
        return $this->direction;
    }

    public function setDirection(int $direction): self
    {
        $this->direction = $direction;

        return $this;
    }

    public function getSender(): ?int
    {
        return $this->sender;
    }

    public function setSender(int $sender): self
    {
        $this->sender = $sender;

        return $this;
    }

    public function getSenderPort(): ?int
    {
        return $this->sender_port;
    }

    public function setSenderPort(int $sender_port): self
    {
        $this->sender_port = $sender_port;

        return $this;
    }

    public function getReceiver(): ?int
    {
        return $this->receiver;
    }

    public function setReceiver(int $receiver): self
    {
        $this->receiver = $receiver;

        return $this;
    }

    public function getReceiverPort(): ?int
    {
        return $this->receiver_port;
    }

    public function setReceiverPort(int $receiver_port): self
    {
        $this->receiver_port = $receiver_port;

        return $this;
    }

    public function getCommand(): ?int
    {
        return $this->command;
    }

    public function setCommand(int $command): self
    {
        $this->command = $command;

        return $this;
    }

    public function getData(): ?int
    {
        return $this->data;
    }

    public function setData(int $data): self
    {
        $this->data = $data;

        return $this;
    }

    public function getRssi(): ?int
    {
        return $this->rssi;
    }

    public function setRssi(int $rssi): self
    {
        $this->rssi = $rssi;

        return $this;
    }

    public function getSnr(): ?float
    {
        return $this->snr;
    }

    public function setSnr(float $snr): self
    {
        $this->snr = $snr;

        return $this;
    }
}
