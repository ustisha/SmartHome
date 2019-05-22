#include "ValueIntInterface.h"

int ValueIntInterface::get() {
    this->read();
    return value;
}
