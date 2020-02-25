#ifndef HANDLERINTERFACE_H
#define HANDLERINTERFACE_H

#include <Arduino.h>

class HandlerInterface {

public:
    virtual void call(uint8_t type, uint8_t idx) = 0;

};

#endif //HANDLERINTERFACE_H
