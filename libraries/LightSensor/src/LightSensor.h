#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensor {
public:

    virtual ~LightSensor() = default;

    virtual void read() = 0;

    float get();

protected:
    float light;
};

#endif //LIGHTSENSOR_H
