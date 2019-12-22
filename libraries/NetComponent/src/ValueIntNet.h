#ifndef VALUEINTNET_H
#define VALUEINTNET_H

#include <Arduino.h>
#include <ValueIntInterface.h>
#include <NetComponent.h>

class ValueIntNet : public NetComponent {
    using NetComponent::NetComponent;
public:
    ValueIntNet(SmartNet *n, uint8_t sp, uint8_t max, ValueIntInterface *vInt);

    void sendCommandData(RadioInterface *n, uint8_t r, uint8_t rp, uint8_t cmd) override;

protected:
    ValueIntInterface *valueInt;

    /**
     * Realisation not needed.
     * @param cmd
     * @param data
     */
    void receiveCommandData(uint8_t cmd, long data) override {};
};

#endif //VALUEINTNET_H
