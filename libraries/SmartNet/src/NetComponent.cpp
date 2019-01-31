#include "NetComponent.h"

NetComponent::NetComponent(uint16_t sp) {
    sport = sp;
}

void NetComponent::addNet(SmartNet *n, uint16_t r, uint16_t rp) {
    net = n;
    receiver = r;
    rport = rp;
}
