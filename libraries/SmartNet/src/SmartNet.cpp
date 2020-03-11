#include "SmartNet.h"
#include <NetComponent.h>

void SmartNet::addNetComponent(NetComponent *nc) {
    if (i >= maxCmp) {
        IF_SERIAL_DEBUG(PSTR("[SmartNet::addNetComponent] Component limit reached\n"));
        return;
    }

    components[i].netComponent = nc;
    IF_SERIAL_DEBUG(printf_P(PSTR("[SmartNet::addNetComponent] Idx: %d\n"), i));
    i++;
}

void SmartNet::commandReceived(Packet *p) {
    for (int i = 0; i < maxCmp; i++) {
        if (components[i].netComponent != nullptr) {
            components[i].netComponent->receiveHandle(p->getReceiverPort(), p->getCommand(), p->getData());
        }
    }
}

void SmartNet::sendInfo(RadioInterface *n, long data, uint8_t cmd) {
    Packet p{};
    p.setSender(sender);
    p.setSenderPort(PORT_INFO);
    p.setReceiver(GATEWAY);
    p.setReceiverPort(PORT_HTTP_HANDLER);
    p.setCommand(cmd);
    p.setData(data);

    n->sendData(&p);
}

