#ifndef VALUEINTERFACE_H
#define VALUEINTERFACE_H

#include <Arduino.h>
#include <DebugLog.h>

class ValueIntInterface {

public:

    int get() {
        this->read();
        return value;
    }

protected:

    int value = 0;

    virtual int read() = 0;
};

#endif VALUEINTERFACE_H
