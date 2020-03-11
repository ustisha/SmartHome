//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <DisplaySH1106.h>
#include <DHTAdapter.h>
#include <THNet.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <Button.h>
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

#define BUTTON1 A0
#define BUTTON2 A1
#define PIR1 A2
#define PIR2 A3
#define RELAY1 3
#define RELAY2 4
#define DHT22 5
#define TFT_CS 10
#define TFT_RST 7 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 6

#define SENSOR_INTERVAL 30

const int memBase = 20;

DisplaySH1106 *oled;
SmartNet *net;
RF24Net *rf24Net;
DHTAdapter *dht22;
THNet *thNet;
LightController *lightController1, *lightController2;
LightControllerNet *lightControllerNet1, *lightControllerNet2;
PIR *pir1, *pir2;
Relay *r1, *r2;
Button *btnLight1, *btnLight2;

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

    net = new SmartNet(BATHROOM, 4);
    rf24Net = new RF24Net(net, BATHROOM, radio);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    oled = new DisplaySH1106(TFT_CS, TFT_DC, TFT_RST);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Display initialized\n")));
    net->sendInfo(rf24Net, INFO_DISPLAY_INIT_COMPLETED);

    dht22 = new DHTAdapter(DHT22, DHT_TYPE_22);
    dht22->setPollInterval(SENSOR_INTERVAL);
    oled->addModule(dht22);
    thNet = new THNet(net, PORT_DHT22, 2, dht22);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);

    btnLight1 = new Button(BUTTON1, 3);
    pir1 = new PIR(PIR1, 1);
    r1 = new Relay(RELAY1, false);
    lightController1 = new LightController();
    lightController1->addRelay(r1);
    lightController1->addPir(pir1);
    lightController1->addButton(btnLight1);
    lightControllerNet1 = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController1);
    lightController1->addNet(rf24Net, lightControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

    btnLight2 = new Button(BUTTON2, 3);
    pir2 = new PIR(PIR2, 1);
    r2 = new Relay(RELAY2, false);
    lightController2 = new LightController();
    lightController2->addRelay(r2);
    lightController2->addPir(pir2);
    lightController2->addButton(btnLight2);
    lightControllerNet2 = new LightControllerNet(net, PORT_LIGHT_CTRL_01, 1, lightController2);
    lightController2->addNet(rf24Net, lightControllerNet2, GATEWAY, PORT_HTTP_HANDLER);

    oled->addModule(lightController2);
    oled->addModule(lightController1);

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

