#ifndef NETINFO_H
#define NETINFO_H

#include <Arduino.h>
#include <NetComponent.h>
#include <Net.h>

class InfoNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    InfoNet(uint16_t p, SmartNet *n);

    virtual ~InfoNet() {};

    void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

    void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd, long value);

protected:
    RadioInterface *radio;
    long info = 0;
};

#endif //NETINFO_H
