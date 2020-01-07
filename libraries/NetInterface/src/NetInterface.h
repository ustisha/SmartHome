#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#include <Arduino.h>
#include <DebugLog.h>
#include <RadioInterface.h>
#include <NetComponent.h>

class NetInterface {
public:
    void addNet(RadioInterface *radioInterface, NetComponent *netCmp, uint8_t receiver, uint8_t receiverPort);

    void sendCommand(uint8_t cmd);

protected:
    NetComponent *netComponent = nullptr;
    RadioInterface *radio;
    uint8_t r;
    uint8_t rp;
};

#endif //NETINTERFACE_H
