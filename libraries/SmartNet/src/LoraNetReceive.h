#ifndef LORANETRECEIVE_H
#define LORANETRECEIVE_H

#include <Arduino.h>
#include <LoraNet.h>

class LoraNetReceive : public LoraNet {
public:

    void sendData(Packet *p) override;
};

#endif //LORANETRECEIVE_H
