#ifndef LORANET_H
#define LORANET_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <RadioInterface.h>
#include <LoRa.h>
#include <SmartNet.h>
#include <Format.h>

class LoraNet : public RadioInterface {
    using RadioInterface::RadioInterface;
    const uint16_t WAIT_TRANSMITTING = 500;
public:

    LoraNet(SmartNet *net, uint8_t ss = LORA_DEFAULT_SS_PIN, uint8_t reset = LORA_DEFAULT_RESET_PIN,
            uint8_t dio0 = LORA_DEFAULT_DIO0_PIN);

    void sendData(Packet *p) override;

    void onReceiveFunc(void(*callback)(int));

    void receiveProcess(SmartNet *net, int packetSize, uint8_t receiver);
};

#endif //LORANET_H
