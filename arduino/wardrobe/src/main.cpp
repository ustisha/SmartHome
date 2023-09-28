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
#include <BinaryReader.h>
#include <ValueIntNet.h>
#include <LightController.h>
#include <LightControllerNet.h>
#include <CtrlRelay.h>

#define MOTION1 2
#define MOTION2 3
#define DOOR 4
#define RELAY1 5
#define RELAY2 6
#define DETECT_DELAY 5 //30

SmartNet *net;
RF24Net *rf24Net;

BinaryReader *door1;
ValueIntNet *doorNet1;
LightController *lightController1, *lightController2;
LightControllerNet *lightControllerNet1, *lightControllerNet2;
Relay *r1, *r2;
MotionCtrl *m1, *m2;
MotionNet *motionNet1, *motionNet2;

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

    net = new SmartNet(WARDROBE, 5);
    rf24Net = new RF24Net(net, WARDROBE, radio, RF24_PA_MAX);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    door1 = new BinaryReader(DOOR, false);
    doorNet1 = new ValueIntNet(net, PORT_VALUE_2, 1, door1);
    door1->addNet(rf24Net, doorNet1, GATEWAY, PORT_HTTP_HANDLER);

    m1 = new MotionCtrl(MOTION1, DETECT_DELAY, 1, false);
    motionNet1 = new MotionNet(net, PORT_MOTION_1, 1, m1);
    m1->addNet(rf24Net, motionNet1, GATEWAY, PORT_HTTP_HANDLER);

    r1 = new Relay(RELAY1, false);
    lightController1 = new LightController();
    lightController1->addRelay(r1);
    lightController1->addMotion(m1);


    lightControllerNet1 = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController1);
    lightController1->addNet(rf24Net, lightControllerNet1, GATEWAY, PORT_HTTP_HANDLER);

    m2 = new MotionCtrl(MOTION2, DETECT_DELAY, 1, false);
    motionNet2 = new MotionNet(net, PORT_MOTION_2, 1, m2);
    m2->addNet(rf24Net, motionNet2, GATEWAY, PORT_HTTP_HANDLER);

    r2 = new Relay(RELAY2, false);
    lightController2 = new LightController();
    lightController2->addRelay(r2);
    lightController2->addMotion(m2);
    lightControllerNet2 = new LightControllerNet(net, PORT_LIGHT_CTRL_01, 1, lightController2);
    lightController2->addNet(rf24Net, lightControllerNet2, GATEWAY, PORT_HTTP_HANDLER);

    door1->sendValues();
    m1->sendValues();
    m2->sendValues();
    lightController1->sendValues();
    lightController2->sendValues();

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
    door1->tick();
    m1->tick();
    m2->tick();
    motionNet1->tick();
    motionNet2->tick();
    lightController1->tick();
    lightControllerNet1->tick();
    lightController2->tick();
    lightControllerNet2->tick();
}