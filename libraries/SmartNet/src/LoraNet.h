#ifndef SMARTHOME_LORANET_H
#define SMARTHOME_LORANET_H

#include <Arduino.h>
#include <Radio.h>
#include <LoRa.h>

class LoraNet : public Radio {

public:

    explicit LoraNet(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN,
                     int dio0 = LORA_DEFAULT_DIO0_PIN);

    bool setup() override;

    void sendData(Packet p) override;

};

#endif //SMARTHOME_LORANET_H
