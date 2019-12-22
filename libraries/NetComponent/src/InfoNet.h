#ifndef NETINFO_H
#define NETINFO_H

#include <Arduino.h>
#include <NetComponent.h>
#include <RadioInterface.h>
#include <Net.h>

class InfoNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd, long value);

protected:
    RadioInterface *radio;
    long info = 0;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};
};

#endif //NETINFO_H
