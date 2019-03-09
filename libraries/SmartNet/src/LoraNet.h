#ifndef LORANET_H
#define LORANET_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Radio.h>
#include <LoRa.h>

union UInt {
    uint16_t i = 0;
    uint8_t b[sizeof(uint16_t)];

    UInt(uint16_t value) : i(value) {

    };
};

union Long {
    long i = 0;
    uint8_t b[sizeof(long)];

    Long(long value) : i(value) {

    };
};

class LoraNet : public Radio {
    using Radio::Radio;
    const uint16_t WAIT_TRANSMITTING = 2000;
public:

    LoraNet(uint8_t ss = LORA_DEFAULT_SS_PIN, uint8_t reset = LORA_DEFAULT_RESET_PIN,
            uint8_t dio0 = LORA_DEFAULT_DIO0_PIN);

    virtual ~LoraNet() {};

    bool setup() override;

    void sendData(Packet &p) override;

};

#endif //LORANET_H
