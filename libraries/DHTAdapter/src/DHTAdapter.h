#ifndef DHTADAPTER_H
#define DHTADAPTER_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Format.h>
#include <THInterface.h>
#include <dht_nonblocking.h>

class DHTAdapter : public THInterface {

public:
    DHTAdapter(uint8_t pin, uint8_t type);

    void read() override;

    void tick();

protected:
    bool reading = false;
    DHT_nonblocking *dht;
};


#endif // DHTADAPTER_H
