#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#include <Arduino.h>
#include <DebugLog.h>
#include <RadioInterface.h>
#include <NetComponent.h>

class NetInterface {
public:
    void addNet(RadioInterface *radioInterface, NetComponent *netCmp, uint8_t receiver, uint8_t receiverPort);

protected:
    NetComponent *netComponent;
    RadioInterface *radio;
    bool networking = false;
    uint8_t r, rp;
};

#endif //NETINTERFACE_H
