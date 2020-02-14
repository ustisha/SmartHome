//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <BME280Adapter.h>
#include <THPNet.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <PIR.h>
#include <LightController.h>
#include <LightControllerNet.h>
#include <EEPROMex.h>
#include <TempController.h>
#include <TempControllerNet.h>

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

#define PIR1 4
#define PIR2 5
#define RELAY1 6
#define RELAY2 7

#define SERV1 3

#define SENSOR_INTERVAL 30

const int memBase = 20;

SmartNet *net;
RF24Net *rf24Net;
BME280Adapter *bmeThp;
THPNet *thpNet;
Servo *serv1;
TempController *tempController;
TempControllerNet *tempControllerNet;
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
    net = new SmartNet(COLD_CHAMBER, 4);
    rf24Net = new RF24Net(net, COLD_CHAMBER, radio);

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    bmeThp = new BME280Adapter(Wire, 0x76);
    bmeThp->setPollInterval(SENSOR_INTERVAL);
    thpNet = new THPNet(net, PORT_BME280, 3, bmeThp);
    if (bmeThp->getStatus() < 0) {
        // BME280 error
        net->sendInfo(rf24Net, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    serv1 = new Servo();
    serv1->attach(SERV1, 680, 2400);

    tempController = new TempController(bmeThp, 0, 1, 20.0, 30.0);
    tempController->addServo(serv1, 0, TempController::TYPE_ABOVE_DOWN_LIMIT, 18, 155, 2.4);
    tempControllerNet = new TempControllerNet(net, PORT_TEMP_CTRL, 1, tempController);
    tempController->addNet(rf24Net, tempControllerNet, GATEWAY, PORT_HTTP_HANDLER);

    pir1 = new PIR(PIR1);
    r1 = new Relay(RELAY1);
    lightController1 = new LightController();
    lightController1->addRelay(r1);
    lightController1->addSwitch(pir1);
    lightControllerNet1 = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController1);
    lightController1->addNet(rf24Net, lightControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

    pir2 = new PIR(PIR2);
    r2 = new Relay(RELAY2);
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
    bmeThp->tick();

    rf24Net->tick();
    thpNet->tick();
    tempController->tick();
    tempControllerNet->tick();

    lightController1->tick();
    lightControllerNet1->tick();
    lightController2->tick();
    lightControllerNet2->tick();
}
