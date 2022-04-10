//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>

#ifdef SERIAL_DEBUG

#include <DebugFunc.h>

#endif

#include <NetInterface.h>
#include <DS18B20Adapter.h>
#include <RF24Net.h>
#include <TNet.h>
#include <Relay.h>
#include <CtrlRelay.h>
#include <TempCtrlRelay.h>
#include <TempCtrlRelayNet.h>

#define RELAY1 5
#define RELAY2 6

#define ONE_WIRE_BUS A5

#define SENSOR_INTERVAL 60
#define POLL_INTERVAL 20

const int memBase = 20;

OneWire oneWire(ONE_WIRE_BUS);

SmartNet *net;
RF24Net *rf24Net;
TNet *tempNetT, *tempNetB;
DS18B20Adapter *dsTempT, *dsTempB;
Relay *r1, *r2;
CtrlRelay *ctrlRelay1, *ctrlRelay2;
TempCtrlRelay *tempCtrlT, *tempCtrlB;
TempCtrlRelayNet *tempCtrlNetT, *tempCtrlNetB;

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

    SPI.begin();
    net = new SmartNet(BATHROOM_FLOOR, 4);
    rf24Net = new RF24Net(net, BATHROOM_FLOOR, radio, RF24_PA_LOW);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    dsTempT = new DS18B20Adapter(&oneWire, 1);
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

    dsTempB = new DS18B20Adapter(&oneWire, 0);
    dsTempB->setPollInterval(POLL_INTERVAL);
    tempNetB = new TNet(net, PORT_18B20_2, 1, dsTempB);
    if (!dsTempB->getStatus()) {
        // DS18B20 error
        net->sendInfo(rf24Net, INFO_ERROR_DS18B20);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] B DS18B20 error\n")));
    } else {
        dsTempB->read();
        tempNetB->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    }

    r1 = new Relay(RELAY1, false);
    ctrlRelay1 = new CtrlRelay(r1);
    tempCtrlT = new TempCtrlRelay(reinterpret_cast<THInterface *>(dsTempT),
                                  ctrlRelay1,
                                  27.0,

                                  28.0,
                                  TempCtrl::TYPE_TEMPERATURE
                                  | TempCtrl::TYPE_BELOW_DOWN_LIMIT);
    tempCtrlNetT = new TempCtrlRelayNet(net, PORT_TEMP_CTRL, 1, tempCtrlT);
    tempCtrlT->addNet(rf24Net, tempCtrlNetT, GATEWAY, PORT_HTTP_HANDLER);
    tempCtrlT->sendValues();

    r2 = new Relay(RELAY2, false);
    ctrlRelay2 = new CtrlRelay(r2);
    tempCtrlB = new TempCtrlRelay(reinterpret_cast<THInterface *>(dsTempB),
                                  ctrlRelay2,
                                  27.0,
                                  28.0,
                                  TempCtrl::TYPE_TEMPERATURE
                                  | TempCtrl::TYPE_BELOW_DOWN_LIMIT);
    tempCtrlNetB = new TempCtrlRelayNet(net, PORT_TEMP_CTRL_2, 1, tempCtrlB);
    tempCtrlB->addNet(rf24Net, tempCtrlNetB, GATEWAY, PORT_HTTP_HANDLER);
    tempCtrlB->sendValues();

    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop()
{
    rf24Net->tick();

    dsTempT->tick();
    dsTempB->tick();
    tempNetT->tick();
    tempNetB->tick();
    tempCtrlT->tick(0);
    tempCtrlB->tick(0);
    tempCtrlNetT->tick();
    tempCtrlNetB->tick();
}