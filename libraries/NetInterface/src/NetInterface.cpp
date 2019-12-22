#include "NetInterface.h"

void NetInterface::addNet(RadioInterface *radioInterface, NetComponent *netCmp, uint8_t receiver, uint8_t receiverPort) {
    radio = radioInterface;
    netComponent = netCmp;
    r = receiver;
    rp = receiverPort;
    networking = true;
}
