#include "TemperatureNet.h"

TemperatureNet::TemperatureNet(uint16_t p, Temperature *t) : NetComponent(p) {
    temperature = t;
}
