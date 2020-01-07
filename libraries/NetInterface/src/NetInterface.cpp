#include "NetInterface.h"

void NetInterface::addNet(RadioInterface *radioInterface, NetComponent *netCmp, uint8_t receiver, uint8_t receiverPort) {
    radio = radioInterface;
    netComponent = netCmp;
    r = receiver;
    rp = receiverPort;
}

void NetInterface::sendCommand(uint8_t cmd) {
    if (nullptr != netComponent) {
        netComponent->sendCommandData(radio, r, rp, cmd);
    }
}
