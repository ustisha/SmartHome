//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Config.h>

#ifdef SERIAL_DEBUG

#include <DebugFunc.h>

#endif

#include <RF24Net.h>
#include <OneWire.h>
#include <DHTAdapter.h>
#include <DS18B20Adapter.h>
#include <AnalogReader.h>
#include <Relay.h>
#include <CtrlRelay.h>
#include <CtrlRelayNet.h>
#include <TksM8.h>
#include <MotionCtrl.h>
#include <TempCtrlRelay.h>
#include <LightController.h>
#include <SmartNet.h>
#include <TNet.h>
#include <THNet.h>
#include <ValueIntNet.h>
#include <MotionNet.h>
#include <TempCtrlRelayNet.h>
#include <LightControllerNet.h>
#include <TempCtrlGroup.h>
#include <TempCtrlGroupNet.h>

#define ONE_WIRE_BUS 3
#define DHT22_PIN 5
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
#define SENSOR_INTERVAL 60
#define MOISTURE_INTERVAL 60
#define POLL_INTERVAL 60

const int memBase = 20;

SmartNet *net;
RF24Net *rf24Net;
THNet *thNet;
TNet *tempNetT;
DHTAdapter *dht22;
DS18B20Adapter *dsTempT;
MotionCtrl *m1;
MotionNet *mNet1;
Relay *r1, *r2, *r3, *r4;
CtrlRelay *ctrlRelay1, *ctrlRelay2, *ctrlRelay4, *ctrlActuator1, *ctrlActuator2;
CtrlRelayNet *ctrlRelayNet1;
TksM8 *actuator1, *actuator2;
LightController *lightController;
LightControllerNet *lightControllerNet;
TempCtrlRelay *tempControllerRelay2, *tempControllerRelay4, *tempControllerActuator1, *tempControllerActuator2;
TempCtrlRelayNet *tempControllerNet2, *tempControllerNet4, *tempControllerNetActuator1, *tempControllerNetActuator2;
TempCtrlGroup *tempCtrlGroup;
TempCtrlGroupNet *tempCtrlGroupNet;
AnalogReader *moisture1, *moisture2, *moisture3;
ValueIntNet *moistureNet1, *moistureNet2, *moistureNet3;

OneWire oneWire(ONE_WIRE_BUS);
RF24 radio(RADIO_RESET_CE, RADIO_SS_CSN);


void setup(void)
{
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    EEPROM.setMemPool(memBase, EEPROMSizeATmega1280);

    net = new SmartNet(GREENHOUSE, 13);
    rf24Net = new RF24Net(net, GREENHOUSE, radio, RF24_PA_MAX);
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

    dht22 = new DHTAdapter(DHT22_PIN, DHT22);
    dht22->setPollInterval(POLL_INTERVAL);
    thNet = new THNet(net, PORT_DHT22, 2, dht22);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);

    moisture1 = new AnalogReader(MOISTURE_1);
    moisture1->setOutputValues(100, 0);
    moistureNet1 = new ValueIntNet(net, PORT_VALUE, 1, moisture1);
    moistureNet1->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_MOISTURE, MOISTURE_INTERVAL);
    moisture1->addNet(rf24Net, moistureNet1, GATEWAY, PORT_HTTP_HANDLER);
    moisture1->sendValues();

    moisture2 = new AnalogReader(MOISTURE_2);
    moisture2->setOutputValues(100, 0);
    moistureNet2 = new ValueIntNet(net, PORT_VALUE_2, 1, moisture2);
    moistureNet2->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_MOISTURE, MOISTURE_INTERVAL);
    moisture2->addNet(rf24Net, moistureNet2, GATEWAY, PORT_HTTP_HANDLER);
    moisture2->sendValues();

    moisture3 = new AnalogReader(MOISTURE_3);
    moisture3->setOutputValues(100, 0);
    moistureNet3 = new ValueIntNet(net, PORT_VALUE_3, 1, moisture3);
    moistureNet3->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_MOISTURE, MOISTURE_INTERVAL);
    moisture3->addNet(rf24Net, moistureNet3, GATEWAY, PORT_HTTP_HANDLER);
    moisture3->sendValues();

    r1 = new Relay(R1);
    r2 = new Relay(R2);
    r3 = new Relay(R3);
    r4 = new Relay(R4);
    ctrlRelay1 = new CtrlRelay(r1);
    ctrlRelay2 = new CtrlRelay(r2);
    ctrlRelay4 = new CtrlRelay(r4);

    ctrlRelayNet1 = new CtrlRelayNet(net, PORT_RELAY_00, 1, ctrlRelay1);
    ctrlRelay1->addNet(rf24Net, ctrlRelayNet1, GATEWAY, PORT_HTTP_HANDLER);

    actuator1 = new TksM8(TKS1_K1, TKS1_K2, TKS1_SW1, TKS1_SW2, 15);
    actuator2 = new TksM8(TKS2_K1, TKS2_K2, TKS2_SW1, TKS2_SW2, 15);
    actuator1->off();
    actuator2->off();
    ctrlActuator1 = new CtrlRelay(actuator1);
    ctrlActuator2 = new CtrlRelay(actuator2);

    m1 = new MotionCtrl(MOTION1, 5, 1);
    mNet1 = new MotionNet(net, PORT_MOTION_1, 1, m1);
    m1->addNet(rf24Net, mNet1, GATEWAY, PORT_HTTP_HANDLER);
    m1->sendValues();

    lightController = new LightController();
    lightController->addRelay(r3);
    lightController->addMotion(m1);
    lightControllerNet = new LightControllerNet(net, PORT_LIGHT_CTRL_00, 1, lightController);
    lightController->addNet(rf24Net, lightControllerNet, GATEWAY, PORT_HTTP_HANDLER);
    lightController->sendValues();

    tempControllerRelay2 = new TempCtrlRelay(
            dht22,
            ctrlRelay2,
            8.0,
            10.0,
            TempCtrlRelay::TYPE_TEMPERATURE | TempCtrl::TYPE_BELOW_DOWN_LIMIT);
    tempControllerRelay4 = new TempCtrlRelay(
            dht22,
            ctrlRelay4,
            6.0,
            8.0,
            TempCtrlRelay::TYPE_TEMPERATURE | TempCtrl::TYPE_BELOW_DOWN_LIMIT);
    tempControllerActuator1 = new TempCtrlRelay(
            dht22,
            ctrlActuator1,
            18.0,
            20.0,
            TempCtrlRelay::TYPE_TEMPERATURE | TempCtrl::TYPE_ABOVE_UP_LIMIT);
    tempControllerActuator2 = new TempCtrlRelay(
            dht22,
            ctrlActuator2,
            26.0,
            24.0,
            TempCtrlRelay::TYPE_TEMPERATURE | TempCtrl::TYPE_ABOVE_UP_LIMIT);

    tempControllerNet2 = new TempCtrlRelayNet(net, PORT_TEMP_CTRL, 1, tempControllerRelay2);
    tempControllerRelay2->addNet(rf24Net, tempControllerNet2, GATEWAY, PORT_HTTP_HANDLER);
    tempControllerNet4 = new TempCtrlRelayNet(net, PORT_TEMP_CTRL_2, 1, tempControllerRelay4);
    tempControllerRelay4->addNet(rf24Net, tempControllerNet4, GATEWAY, PORT_HTTP_HANDLER);
    tempControllerNetActuator1 = new TempCtrlRelayNet(net, PORT_TEMP_CTRL_3, 1, tempControllerActuator1);
    tempControllerActuator1->addNet(rf24Net, tempControllerNetActuator1, GATEWAY, PORT_HTTP_HANDLER);
    tempControllerNetActuator2 = new TempCtrlRelayNet(net, PORT_TEMP_CTRL_4, 1, tempControllerActuator2);
    tempControllerActuator2->addNet(rf24Net, tempControllerNetActuator2, GATEWAY, PORT_HTTP_HANDLER);

    tempCtrlGroup = new TempCtrlGroup(4);
    tempCtrlGroup->addTempCtrl(tempControllerRelay2, 0);
    tempCtrlGroup->addTempCtrl(tempControllerRelay4, 1);
    tempCtrlGroup->addTempCtrl(tempControllerActuator1, 2);
    tempCtrlGroup->addTempCtrl(tempControllerActuator2, 3);

    tempCtrlGroupNet = new TempCtrlGroupNet(net, PORT_TEMP_CTRL_GROUP_1, 1, tempCtrlGroup);
    tempCtrlGroup->addNet(rf24Net, tempCtrlGroupNet, GATEWAY, PORT_HTTP_HANDLER);
    tempCtrlGroup->sendValues();

    tempControllerRelay2->sendValues();
    tempControllerRelay4->sendValues();
    tempControllerActuator1->sendValues();
    tempControllerActuator2->sendValues();

    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop(void)
{
    rf24Net->tick();

    dsTempT->tick();
    tempNetT->tick();
    dht22->tick();
    thNet->tick();

    moistureNet1->tick();
    moistureNet2->tick();
    moistureNet3->tick();

    m1->tick();
    mNet1->tick();
    lightController->tick();
    lightControllerNet->tick();

    actuator1->tick();
    actuator2->tick();
    tempControllerRelay2->tick(0);
    tempControllerRelay4->tick(0);
    tempControllerActuator1->tick(0);
    tempControllerActuator2->tick(0);

    tempControllerNet2->tick();
    tempControllerNet4->tick();
    tempControllerNetActuator1->tick();
    tempControllerNetActuator2->tick();
    tempCtrlGroupNet->tick();

    ctrlRelayNet1->tick(0);
}
