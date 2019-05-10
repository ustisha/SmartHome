#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#include <Arduino.h>
#include <DebugLog.h>
#include <RadioInterface.h>
#include <NetComponent.h>

class NetInterface {
public:
    virtual ~NetInterface() {};

    void addNet(RadioInterface *radio, NetComponent *netComponent, uint8_t r, uint16_t rp);

protected:
    NetComponent *netComponent;
    RadioInterface *radio;
    uint8_t r, rp;
};

#endif //NETINTERFACE_H
