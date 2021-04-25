//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>

#ifdef SERIAL_DEBUG

#include <DebugFunc.h>

#endif

#include <DS18B20Adapter.h>
#include <RF24Net.h>
#include <TNet.h>
#include <TempController.h>
#include <TempControllerNet.h>
#include <Relay.h>

#define RELAY1 A1
#define RELAY2 A2

#define ONE_WIRE_BUS 4
#define RADIO_RESET_CE 9
#define RADIO_SS_CSN 10

#define SENSOR_INTERVAL 60
#define POLL_INTERVAL 60

const int memBase = 20;

OneWire oneWire(ONE_WIRE_BUS);

SmartNet *net;
RF24Net *rf24Net;
TNet *tempNetT, *tempNetB;
DS18B20Adapter *dsTempT, *dsTempB;
TempController *tempControllerT, *tempControllerB;
TempControllerNet *tempControllerNetT, *tempControllerNetB;
Relay *r1, *r2;

RF24 radio(RADIO_RESET_CE, RADIO_SS_CSN);

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
    rf24Net = new RF24Net(net, BATHROOM_FLOOR, radio);
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

    dsTempB = new DS18B20Adapter(&oneWire, 1);
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
    tempControllerT = new TempController(dsTempT, 1, 0, 27.0, 28.0);
    tempControllerT->addRelay(r1, 0, TempController::TYPE_TEMPERATURE
                                     | TempController::TYPE_BELOW_DOWN_LIMIT, 0, 0);
    tempControllerNetT = new TempControllerNet(net, PORT_TEMP_CTRL, 1, tempControllerT);
    tempControllerT->addNet(rf24Net, tempControllerNetT, GATEWAY, PORT_HTTP_HANDLER);

    r2 = new Relay(RELAY2, false);
    tempControllerB = new TempController(dsTempB, 1, 0, 27.0, 28.0);
    tempControllerB->addRelay(r2, 0, TempController::TYPE_TEMPERATURE
                                     | TempController::TYPE_BELOW_DOWN_LIMIT, 0, 0);
    tempControllerNetB = new TempControllerNet(net, PORT_TEMP_CTRL_2, 1, tempControllerB);
    tempControllerB->addNet(rf24Net, tempControllerNetB, GATEWAY, PORT_HTTP_HANDLER);


    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(
            printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop()
{
    dsTempT->tick();
    dsTempB->tick();
    tempNetT->tick();
    tempNetB->tick();
    tempControllerT->tick();
    tempControllerB->tick();
    tempControllerNetT->tick();
    tempControllerNetB->tick();
}