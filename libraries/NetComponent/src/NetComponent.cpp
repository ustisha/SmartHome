#include "NetComponent.h"

NetComponent::NetComponent(uint16_t p) {
    port = p;
}

void NetComponent::addNet(SmartNet *n) {
    net = n;
}
