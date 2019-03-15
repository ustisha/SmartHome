#ifndef LORANETEE_H
#define LORANETEE_H

#include <Arduino.h>
#include <LoraNet.h>

class LoraNetSender : public LoraNet {
    using LoraNet::LoraNet;
public:

    LoraNetSender(uint8_t ss, uint8_t reset, uint8_t dio0);

    void sendData(Packet &p) override;
};

#endif //LORANETEE_H
