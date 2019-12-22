//#define SERIAL_DEBUG

#include <LowPower.h>
#include <DebugLog.h>
#include <Wire.h>
#include <LightSensorNet.h>
#include <BH1750Adapter.h>
#include <DS18B20Adapter.h>
#include <BME280Adapter.h>
#include <InfoNet.h>
#include <TNet.h>
#include <THPNet.h>
#include <SmartNet.h>
#include <RF24NetSleep.h>
#include <Vcc.h>
#include <VccNet.h>

#ifdef IF_SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);

    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

#endif

#define RADIO_RESET_CE 9
#define RADIO_SS_CSN 10

#define ONE_WIRE_BUS 2

#define SENSOR_INTERVAL 120
#define POLL_INTERVAL 110

const uint16_t SLEEP = 4000;
const float vccCorrection = 1.0/1.0;

OneWire oneWire(ONE_WIRE_BUS);

InfoNet *info;
SmartNet *net;
RF24NetSleep *rf24NetSleep;
DS18B20Adapter *owTemp;
TNet *tempNet;
BME280Adapter *bmeThp;
THPNet *thpNet;
BH1750Adapter *bhLight;
LightSensorNet *lightNet;
Vcc *vcc;
VccNet *vccNet;

RF24 radio(RADIO_RESET_CE, RADIO_SS_CSN);

void setup(void) {
    // Sec for stabilization components power
    LowPower.powerStandby(SLEEP_250MS, ADC_OFF, BOD_OFF);

#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started\n")));

    net = new SmartNet(OUTSIDE_TEMP, 5);
    rf24NetSleep = new RF24NetSleep(net, OUTSIDE_TEMP, radio);
    info = new InfoNet(net, PORT_INFO, 0);
    // Info network started
    info->sendCommandData(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    owTemp = new DS18B20Adapter(&oneWire, 0);
    owTemp->setPollInterval(POLL_INTERVAL);
    tempNet = new TNet(net, PORT_18B20, 1, owTemp);
    if (!owTemp->getStatus()) {
        // DS18B20 error
        info->sendCommandData(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_ERROR_DS18B20);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] DS18B20 error\n")));
    } else {
        tempNet->addReceiver(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    }

    bmeThp = new BME280Adapter(Wire, 0x76);
    bmeThp->setPollInterval(POLL_INTERVAL);
    thpNet = new THPNet(net, PORT_BME280, 3, bmeThp);
    if (bmeThp->getStatus() < 0) {
        // BME280 error
        info->sendCommandData(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    bhLight = new BH1750Adapter();
    lightNet = new LightSensorNet(net, PORT_BH1750, 1, bhLight);
    if (!bhLight->getStatus()) {
        // BH1750 error
        info->sendCommandData(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BH1750);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BH1750 error\n")));
    } else {
        lightNet->addReceiver(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_LIGHT, SENSOR_INTERVAL);
    }

    vcc = new Vcc(vccCorrection);
    vccNet = new VccNet(net, PORT_VCC, 1, vcc);
    vccNet->addReceiver(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_VCC, SENSOR_INTERVAL);

    // Info setup completed
    info->sendCommandData(rf24NetSleep, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed\n")));
}

void loop(void) {
#ifdef SERIAL_DEBUG
    Serial.flush();
#endif

    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);

    if (owTemp->getStatus()) {
        owTemp->tick(SLEEP);
    }
    if (bmeThp->getStatus()) {
        bmeThp->tick(SLEEP);
    }

    tempNet->tick(SLEEP);
    thpNet->tick(SLEEP);
    lightNet->tick(SLEEP);
    vccNet->tick(SLEEP);
}

