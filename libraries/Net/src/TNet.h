#ifndef TEMPNET_H
#define TEMPNET_H

#include <Arduino.h>
#include <TInterface.h>
#include <NetComponent.h>

class TNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    TNet(uint16_t p, SmartNet *n, TInterface *t);

    virtual ~TNet() {};

    void sendCommandData(RadioInterface *n, uint8_t r, uint16_t rp, uint8_t cmd) override;

protected:
    TInterface *temperature;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};

};

#endif //TEMPNET_H
