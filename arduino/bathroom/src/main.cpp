//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <DHTAdapter.h>
#include <THNet.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <PIR.h>
#include <LightController.h>
#include <LightControllerNet.h>
#include <EEPROMex.h>

#ifdef SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);
    Serial.flush();
    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

#endif

#define PIR1 A0
#define PIR2 A1
#define RELAY1 6
#define RELAY2 7

#define DHT22 5
#define SENSOR_INTERVAL 30

const int memBase = 20;

SmartNet *net;
RF24Net *rf24Net;
DHTAdapter *dht22;
THNet *thNet;
LightController *lightController1, *lightController2;
LightControllerNet *lightControllerNet1, *lightControllerNet2;
PIR *pir1, *pir2;
Relay *r1, *r2;

RF24 radio(RF24_DEFAULT_CE, RF24_DEFAULT_CSN);

#ifdef SERIAL_DEBUG
int freeRAM() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

void setup(void) {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    EEPROM.setMemPool(memBase, EEPROMSizeATmega328);

    SPI.begin();
    net = new SmartNet(BATHROOM, 4);
    rf24Net = new RF24Net(net, BATHROOM, radio);

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    dht22 = new DHTAdapter(DHT22, DHT_TYPE_22);
    dht22->setPollInterval(SENSOR_INTERVAL);
    thNet = new THNet(net, PORT_DHT22, 2, dht22);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);

    pir1 = new PIR(PIR1);
    r1 = new Relay(RELAY1, false);
    lightController1 = new LightController();
    lightController1->addRelay(r1);
    lightController1->addSwitch(pir1);
    lightControllerNet1 = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController1);
    lightController1->addNet(rf24Net, lightControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

    pir2 = new PIR(PIR2);
    r2 = new Relay(RELAY2, false);
    lightController2 = new LightController();
    lightController2->addRelay(r2);
    lightController2->addSwitch(pir2);
    lightControllerNet2 = new LightControllerNet(net, PORT_LIGHT_CTRL_01, 1, lightController2);
    lightController2->addNet(rf24Net, lightControllerNet2, GATEWAY, PORT_HTTP_HANDLER);

    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop(void) {
#ifdef SERIAL_DEBUG
    Serial.flush();
#endif
    dht22->tick();

    rf24Net->tick();
    thNet->tick();

    lightController1->tick();
    lightControllerNet1->tick();
    lightController2->tick();
    lightControllerNet2->tick();
}

