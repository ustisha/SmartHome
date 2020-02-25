//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <Servo.h>
#include <DHTAdapter.h>
#include <THNet.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <Button.h>
#include <PIR.h>
#include <TempController.h>
#include <TempControllerNet.h>
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
#define RELAY1 6
#define DHT22 5
#define SERV1 3

#define SENSOR_INTERVAL 30

const int memBase = 20;

SmartNet *net;
RF24Net *rf24Net;
DHTAdapter *dht22;
THNet *thNet;
Servo *serv1;
TempController *tempController1;
TempControllerNet *tempControllerNet1;
LightController *lightController1;
LightControllerNet *lightControllerNet1;
PIR *pir1;
Relay *r1;
Button *btnLight1, *btnServo1;

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

    int8_t serv1Idx = 0;
    btnServo1 = new Button(BUTTON2, 3);
    serv1 = new Servo();
    serv1->attach(SERV1, 680, 2400);
    tempController1 = new TempController(dht22, 0, 1, 70.0, 100.0);
    tempController1->addServo(serv1, serv1Idx,
                             TempController::TYPE_ABOVE_DOWN_LIMIT | TempController::TYPE_HUMIDITY,
                                         0, 90, 2.4);
    tempController1->addServoButton(serv1Idx, btnServo1);
    tempControllerNet1 = new TempControllerNet(net, PORT_TEMP_CTRL, 1, tempController1);
    tempController1->addNet(rf24Net, tempControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

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
    tempController1->tick();
    tempControllerNet1->tick();
}

