//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>

#ifdef SERIAL_DEBUG

#include <DebugFunc.h>

#endif

#include <NetInterface.h>
#include <RF24Net.h>
#include <MotionCtrl.h>
#include <MotionNet.h>
#include <THPNet.h>
#include <BME280Adapter.h>
#include <AnalogReader.h>
#include <BinaryReader.h>
#include <ValueIntNet.h>
#include <LightController.h>
#include <LightControllerNet.h>
#include <CtrlRelay.h>
#include <CtrlRelayNet.h>

#define MQ2 A7
#define MOTION 6
#define LIGHT 2
#define RELAY1 A0
#define RELAY3 9
#define RELAY4 10

#define SENSOR_INTERVAL 30
#define POLL_INTERVAL 30
#define ANALOG_POLL_INTERVAL 5
#define BINARY_POLL_INTERVAL 1

const int memBase = 20;

SmartNet *net;
RF24Net *rf24Net;
THPNet *thpNet;
BME280Adapter *bmeThp;
AnalogReader *mq2;
ValueIntNet *mq2Net;
LightController *lightController1;
LightControllerNet *lightControllerNet1;
Relay *r1, *r3, *r4;
BinaryReader *light1;
ValueIntNet *lightNet1;
MotionCtrl *m1;
MotionNet *motionNet1;
CtrlRelay *ctrlRelay3, *ctrlRelay4;
CtrlRelayNet *ctrlRelayNet3, *ctrlRelayNet4;

RF24 radio(RF24_DEFAULT_CE, RF24_DEFAULT_CSN);

void setup()
{
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    EEPROM.setMemPool(memBase, EEPROMSizeATmega328);

    net = new SmartNet(GARAGE, 7);
    rf24Net = new RF24Net(net, GARAGE, radio, RF24_PA_MAX);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    mq2 = new AnalogReader(MQ2);
    mq2Net = new ValueIntNet(net, PORT_VALUE, 1, mq2);
    mq2Net->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_VALUE, ANALOG_POLL_INTERVAL);

    light1 = new BinaryReader(LIGHT, true);
    lightNet1 = new ValueIntNet(net, PORT_VALUE_2, 1, light1);
    lightNet1->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_VALUE, BINARY_POLL_INTERVAL);

    m1 = new MotionCtrl(MOTION, 5, 1, false);
    motionNet1 = new MotionNet(net, PORT_MOTION_1, 1, m1);
    m1->addNet(rf24Net, motionNet1, GATEWAY, PORT_HTTP_HANDLER);

    r1 = new Relay(RELAY1, false);
    lightController1 = new LightController();
    lightController1->addRelay(r1);
    lightController1->addMotion(m1);
    lightControllerNet1 = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController1);
    lightController1->addNet(rf24Net, lightControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

    r3 = new Relay(RELAY3, true);
    ctrlRelay3 = new CtrlRelay(r3);
    ctrlRelayNet3 = new CtrlRelayNet(net, PORT_RELAY_03, 1, ctrlRelay3);
    ctrlRelay3->addNet(rf24Net, ctrlRelayNet3, GATEWAY, PORT_HTTP_HANDLER);

    r4 = new Relay(RELAY4, true);
    ctrlRelay4 = new CtrlRelay(r4);
    ctrlRelayNet4 = new CtrlRelayNet(net, PORT_RELAY_04, 1, ctrlRelay4);
    ctrlRelay4->addNet(rf24Net, ctrlRelayNet4, GATEWAY, PORT_HTTP_HANDLER);


    bmeThp = new BME280Adapter(Wire, BME280_ADDRESS_ALTERNATE);
    bmeThp->setPollInterval(POLL_INTERVAL);
    thpNet = new THPNet(net, PORT_BME280, 3, bmeThp);
    if (bmeThp->getStatus() <= 0) {
        // BME280 error
        net->sendInfo(rf24Net, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    mq2->sendValues();
    lightController1->sendValues();
    ctrlRelay3->sendValues();
    ctrlRelay4->sendValues();
    m1->sendValues();
    light1->sendValues();
    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop()
{
#ifdef SERIAL_DEBUG
    Serial.flush();
//    net->tick();
#endif
    rf24Net->tick();

    bmeThp->tick();
    thpNet->tick();
    mq2Net->tick();
    rf24Net->tick();

    lightNet1->tick();
    lightController1->tick();
    lightControllerNet1->tick();
    rf24Net->tick();

    ctrlRelayNet3->tick();
    ctrlRelayNet4->tick();
    rf24Net->tick();

    m1->tick();
}