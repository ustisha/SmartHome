#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensorInterface {
public:

    virtual ~LightSensorInterface() = default;

    virtual void read() = 0;

    float get();

protected:
    float light;
};

#endif //LIGHTSENSOR_H
