#ifndef NETINFO_H
#define NETINFO_H

#include <Arduino.h>
#include <NetComponent.h>
#include <Net.h>

class NetInfo : public NetComponent {
    using NetComponent::NetComponent;
public:
    NetInfo(uint16_t p, SmartNet *n);

    virtual ~NetInfo() {};

    void sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

    void sendCommandData(Radio *n, uint8_t r, uint16_t rp, uint8_t cmd, long value);

protected:
    Radio *radio;
    long info = 0;
};

#endif //NETINFO_H
