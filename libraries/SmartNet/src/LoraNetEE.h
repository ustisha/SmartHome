#ifndef LORANETEE_H
#define LORANETEE_H

#include <Arduino.h>
#include <LoraNet.h>

class LoraNetEE : public LoraNet {
    using LoraNet::LoraNet;
public:

    LoraNetEE(uint8_t ss, uint8_t reset, uint8_t dio0);

    void sendData(Packet &p) override;
};

#endif //LORANETEE_H
