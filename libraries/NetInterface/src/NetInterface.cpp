#include "NetInterface.h"

void NetInterface::addNet(RadioInterface *radio, NetComponent *netComponent, uint8_t r, uint16_t rp) {
    this->radio = radio;
    this->netComponent = netComponent;
    this->r = r;
    this->rp = rp;
}
