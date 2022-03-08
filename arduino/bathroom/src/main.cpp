#include <Arduino.h>
#include <DebugLog.h>
#include <BathroomDisplay.h>
#include <DHTAdapter.h>
#include <THNet.h>
#include <TVirtual.h>
#include <TNet.h>
#include <ValueIntNet.h>
#include <WashingMachineNet.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <Button.h>
#include <Motion.h>
#include <LightController.h>
#include <LightControllerNet.h>
#include <EEPROMex.h>
#ifdef VIBRO_ENABLED
#include <Vibro.h>
#endif

//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);
    Serial.flush();
    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

int freeRAM() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

#define BUTTON1 A1
#define BUTTON2 A0
#define PIR1 2
#define PIR2 4
#define RELAY1 A2
#define RELAY2 A3
#define VIBRO_PIN A4
#define DHT22_PIN 9
#define TFT_CS 23
#define TFT_RST (-1) // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 22

#define SENSOR_INTERVAL 10

BathroomDisplay *led;
SmartNet *net;
RF24Net *rf24Net;
DHTAdapter *dht22;
THNet *thNet;
TVirtual *floor1, *floor2;
TNet *floorNet1, *floorNet2;
WashingMachine *wm;
WashingMachineNet *wmNet;
ValueInt *value1, *value2;
ValueIntNet *valueNet1, *valueNet2;
LightController *lightController1, *lightController2;
LightControllerNet *lightControllerNet1, *lightControllerNet2;
Motion *pir1, *pir2;
Relay *r1, *r2;
Button *btnLight1, *btnLight2;
#ifdef VIBRO_ENABLED
Vibro *vibro;
#endif

RF24 radio(RF24_DEFAULT_CE, RF24_DEFAULT_CSN);

void setup(void) {
#ifdef SERIAL_DEBUG
#if defined(__AVR_ATmega4808__)
    delay(2000);
#endif
    Serial.begin(57600);
    while (!Serial);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    EEPROM.setMemPool(0, 256);

    net = new SmartNet(BATHROOM, 9);
    rf24Net = new RF24Net(net, BATHROOM, radio);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    led = new BathroomDisplay(TFT_CS, TFT_DC, TFT_RST);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Display initialized\n")));
    net->sendInfo(rf24Net, INFO_DISPLAY_INIT_COMPLETED);

    dht22 = new DHTAdapter(DHT22_PIN, DHT22);
    dht22->setPollInterval(SENSOR_INTERVAL);
    led->addModule(dht22);

    thNet = new THNet(net, PORT_DHT22, 2, dht22);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);

#ifdef VIBRO_ENABLED
    vibro = new Vibro(VIBRO_PIN);
#endif
    btnLight1 = new Button(BUTTON1, 3);
#ifdef VIBRO_ENABLED
    btnLight1->addVibro(vibro);
#endif
    pir1 = new Motion(PIR1, 1);
    r1 = new Relay(RELAY1, false);
    lightController1 = new LightController();
    lightController1->addRelay(r1);
    lightController1->addMotion(pir1);
    lightController1->addButton(btnLight1);
    lightControllerNet1 = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController1);
    lightController1->addNet(rf24Net, lightControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

    btnLight2 = new Button(BUTTON2, 3);
#ifdef VIBRO_ENABLED
    btnLight2->addVibro(vibro);
#endif
    pir2 = new Motion(PIR2, 1);
    r2 = new Relay(RELAY2, false);
    lightController2 = new LightController();
    lightController2->addRelay(r2);
    lightController2->addMotion(pir2);
    lightController2->addButton(btnLight2);
    lightControllerNet2 = new LightControllerNet(net, PORT_LIGHT_CTRL_01, 1, lightController2);
    lightController2->addNet(rf24Net, lightControllerNet2, GATEWAY, PORT_HTTP_HANDLER);

    led->addModule(lightController2);
    led->addModule(lightController1);

    floor1 = new TVirtual();
    floor2 = new TVirtual();
    led->addFloorModule(floor1);
    led->addFloorModule(floor2);
    floorNet1 = new TNet(net, PORT_TVIRTUAL_1, 1, floor1);
    floorNet2 = new TNet(net, PORT_TVIRTUAL_2, 1, floor2);

    value1 = new ValueInt();
    value2 = new ValueInt();
    led->addValueModule(value1);
    led->addValueModule(value2);
    valueNet1 = new ValueIntNet(net, PORT_VALUE_2, 1, value1);
    valueNet2 = new ValueIntNet(net, PORT_VALUE_3, 1, value2);

    wm = new WashingMachine();
    led->addWashingModule(wm);
    wmNet  = new WashingMachineNet(net, PORT_WASHING_MACHINE, 1, wm);

    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop(void) {
#ifdef SERIAL_DEBUG
    Serial.flush();
    net->tick();
#endif
    dht22->tick();

    rf24Net->tick();
    thNet->tick();

    lightController1->tick();
    lightControllerNet1->tick();
    lightController2->tick();
    lightControllerNet2->tick();
}

