//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>

#ifdef SERIAL_DEBUG

#include <DebugFunc.h>

#endif

#include <RF24Net.h>
#include <OneWire.h>
#include <DHTAdapter.h>
#include <DS18B20Adapter.h>
#include <AnalogReader.h>
#include <Relay.h>
#include <RelayNet.h>
#include <TksM8.h>
#include <Motion.h>
#include <TempCtrlRelay.h>
#include <LightController.h>
#include <SmartNet.h>
#include <TNet.h>
#include <THNet.h>
#include <ValueIntNet.h>
#include <MotionNet.h>
#include <TempCtrlRelayNet.h>
#include <LightControllerNet.h>

#define ONE_WIRE_BUS 3
#define DHT22 5
#define RADIO_RESET_CE 48
#define RADIO_SS_CSN 49

#define MOISTURE_1 63 //A9
#define MOISTURE_2 65 //A11
#define MOISTURE_3 67 //A13
#define R1 20
#define R2 22
#define R3 24
#define R4 26
#define MOTION1 31
#define TKS1_K1 12
#define TKS1_K2 14
#define TKS1_SW1 16
#define TKS1_SW2 18
#define TKS2_K1 8
#define TKS2_K2 10
#define TKS2_SW1 28
#define TKS2_SW2 30

// @todo 10 is debug value
#define SENSOR_INTERVAL 30
#define MOISTURE_INTERVAL 10
#define POLL_INTERVAL 30

const int memBase = 20;

SmartNet *net;
RF24Net *rf24Net;
THNet *thNet;
TNet *tempNetT;
DHTAdapter *dht22;
DS18B20Adapter *dsTempT;
Motion *m1;
MotionNet *mNet1;
Relay *r1, *r2, *r3, *r4;
RelayNet *relayNet1;
TksM8 *actuator1, *actuator2;
LightController *lightController;
LightControllerNet *lightControllerNet;
TempCtrlRelay *tempController;
TempCtrlRelayNet *tempControllerNet;
AnalogReader *moisture1, *moisture2, *moisture3;
ValueIntNet *moistureNet1, *moistureNet2, *moistureNet3;

OneWire oneWire(ONE_WIRE_BUS);
RF24 radio(RADIO_RESET_CE, RADIO_SS_CSN);


void setup(void) {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    EEPROM.setMemPool(memBase, EEPROMSizeATmega1280);

    net = new SmartNet(GREENHOUSE, 9);
    rf24Net = new RF24Net(net, GREENHOUSE, radio);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    dsTempT = new DS18B20Adapter(&oneWire, 0);
    dsTempT->setPollInterval(POLL_INTERVAL);
    tempNetT = new TNet(net, PORT_18B20, 1, dsTempT);
    if (!dsTempT->getStatus()) {
        // DS18B20 error
        net->sendInfo(rf24Net, INFO_ERROR_DS18B20);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] T DS18B20 error\n")));
    } else {
        dsTempT->read();
        tempNetT->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    }

    dht22 = new DHTAdapter(DHT22, DHT_TYPE_22);
    dht22->setPollInterval(POLL_INTERVAL);
    thNet = new THNet(net, PORT_DHT22, 2, dht22);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);

    moisture1 = new AnalogReader(MOISTURE_1);
    // max 637 min 310
    moisture1->setMapValues(310, 640, 100, 0);
    moisture1->enableFilter();
    moistureNet1 = new ValueIntNet(net, PORT_VALUE, 1, moisture1);
    moistureNet1->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_MOISTURE, MOISTURE_INTERVAL);

    moisture2 = new AnalogReader(MOISTURE_2);
    // max 630 min 300
    moisture2->setMapValues(300, 630, 100, 0);
    moisture2->enableFilter();
    moistureNet2 = new ValueIntNet(net, PORT_VALUE_2, 1, moisture2);
    moistureNet2->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_MOISTURE, MOISTURE_INTERVAL);

    moisture3 = new AnalogReader(MOISTURE_3);
    // max 630 min 305
    moisture3->setMapValues(305, 630, 100, 0);
    moisture3->enableFilter();
    moistureNet3 = new ValueIntNet(net, PORT_VALUE_3, 1, moisture3);
    moistureNet3->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_MOISTURE, MOISTURE_INTERVAL);

    r1 = new Relay(R1);
    r2 = new Relay(R2);
    r3 = new Relay(R3);
    r4 = new Relay(R4);
    r1->off();
    r2->off();
    r3->off();
    r4->off();

    relayNet1 = new RelayNet(net, PORT_RELAY_1, 1, r1);
    r1->addNet(rf24Net, relayNet1, GATEWAY, PORT_HTTP_HANDLER);

    actuator1 = new TksM8(TKS1_K1, TKS1_K2, TKS1_SW1, TKS1_SW2, 15);
    actuator2 = new TksM8(TKS2_K1, TKS2_K2, TKS2_SW1, TKS2_SW2, 15);
    actuator1->off();
    actuator2->off();

    m1 = new Motion(MOTION1, 5, 1);
    mNet1 = new MotionNet(net, PORT_MOTION_1, 1, m1);
    m1->addNet(rf24Net, mNet1, GATEWAY, PORT_HTTP_HANDLER);
    lightController = new LightController();
    lightController->setTimeout(30 * 1000);
    lightController->addRelay(r3);
    lightController->addMotion(m1);
    lightControllerNet = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController);
    lightController->addNet(rf24Net, lightControllerNet, GATEWAY, PORT_HTTP_HANDLER);
    lightController->sendValues();

    tempController = new TempCtrlRelay(dht22, 4, 28.0, 30.0);
    tempController->addRelay(r2,
                             0,
                             TempCtrlRelay::TYPE_TEMPERATURE
                             | TempCtrlRelay::TYPE_BELOW_DOWN_LIMIT
                             | TempCtrlRelay::TYPE_ABOVE_DOWN_LIMIT,
                             1,
                             1);
    tempController->addRelay(r4,
                             1,
                             TempCtrlRelay::TYPE_TEMPERATURE
                             | TempCtrlRelay::TYPE_BELOW_DOWN_LIMIT
                             | TempCtrlRelay::TYPE_ABOVE_DOWN_LIMIT,
                             3,
                             2);
    tempController->addRelay(actuator1,
                             2,
                             TempCtrlRelay::TYPE_TEMPERATURE
                             | TempCtrlRelay::TYPE_ABOVE_UP_LIMIT
                             | TempCtrlRelay::TYPE_BELOW_UP_LIMIT,
                             5,
                             5);
    tempController->addRelay(actuator2,
                             3,
                             TempCtrlRelay::TYPE_TEMPERATURE
                             | TempCtrlRelay::TYPE_ABOVE_UP_LIMIT
                             | TempCtrlRelay::TYPE_BELOW_UP_LIMIT,
                             0,
                             5);

    tempController->initDone();
    tempControllerNet = new TempCtrlRelayNet(net, PORT_TEMP_CTRL, 1, tempController);
    tempController->addNet(rf24Net, tempControllerNet, GATEWAY, PORT_HTTP_HANDLER);
    tempController->sendValues();

    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop(void) {
    rf24Net->tick();

    dsTempT->tick();
    tempNetT->tick();
    dht22->tick();
    thNet->tick();

    relayNet1->tick();
    moistureNet1->tick();
    moistureNet2->tick();
    moistureNet3->tick();

    m1->tick();
    mNet1->tick();
    lightController->tick();
    lightControllerNet->tick();

    actuator1->tick();
    actuator2->tick();
    tempController->tick(0);
    tempControllerNet->tick();
}
