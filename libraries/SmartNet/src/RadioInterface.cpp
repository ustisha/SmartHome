#include "RadioInterface.h"

RadioInterface::RadioInterface(SmartNet *net) {
    smartNet = net;
    status = 0;
}

int RadioInterface::getStatus() {
    return status;
}
