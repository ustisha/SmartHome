#ifndef ANALOGREADER_H
#define ANALOGREADER_H

#include <Arduino.h>
#include <ValueIntInterface.h>

class AnalogReader : public ValueIntInterface {
public:
    AnalogReader(uint8_t p);

    int read() override;

protected:
    uint8_t pin;
};

#endif //ANALOGREADER_H
