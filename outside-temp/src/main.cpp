//#define SERIAL_DEBUG

#include <avr/pgmspace.h>
#include <avr/power.h>
#include <LowPower.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <DebugLog.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <LoRa.h>
#include <Net.h>
#include <LightSensorNet.h>
#include <LightSensorInterface.h>
#include <BH1750.h>
#include <BH1750Adapter.h>
#include <DallasTemperature.h>
#include <TInterface.h>
#include <DS18B20Adapter.h>
#include <THInterface.h>
#include <THPInterface.h>
#include <BME280.h>
#include <BME280Adapter.h>
#include <InfoNet.h>
#include <TNet.h>
#include <THPNet.h>
#include <SmartNet.h>
#include <LoraNetEE.h>
#include <Vcc.h>
#include <VccNet.h>

#define LORA_SS 10
#define LORA_RESET 9
#define LORA_DIO0 3
#define ONE_WIRE_BUS 2

#define SENSOR_INTERVAL 30
#define POLL_INTERVAL 20

const uint16_t SLEEP = 1000;
const float vccCorrection = 1.0/1.0;

OneWire oneWire(ONE_WIRE_BUS);

InfoNet *info;
SmartNet *net;
LoraNetEE *loraNetEE;
DS18B20Adapter *owTemp;
TNet *tempNet;
BME280Adapter *bmeThp;
THPNet *thpNet;
BH1750Adapter *bhLight;
LightSensorNet *lightNet;
Vcc *vcc;
VccNet *vccNet;

void setup(void) {
    // Sec for stabilization components power
    LowPower.powerStandby(SLEEP_250MS, ADC_OFF, BOD_OFF);

#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#else
    // Only in production mode. Serial print not working with this settings.
    // clock_prescale_set(clock_div_2);
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started\n")));

    loraNetEE = new LoraNetEE(LORA_SS, LORA_RESET, LORA_DIO0);
    net = new SmartNet(OUTSIDE_TEMP);
    info = new InfoNet(OUTSIDE_TEMP_INFO, net);
    // Info network started
    info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    owTemp = new DS18B20Adapter(&oneWire, 0);
    owTemp->setPollInterval(POLL_INTERVAL);
    tempNet = new TNet(OUTSIDE_TEMP_18B20, net, owTemp);
    if (!owTemp->getStatus()) {
        // DS18B20 error
        info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_ERROR_DS18B20);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] DS18B20 error\n")));
    } else {
        tempNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    }

    bmeThp = new BME280Adapter(Wire, 0x76);
    bmeThp->setPollInterval(POLL_INTERVAL);
    thpNet = new THPNet(OUTSIDE_TEMP_BME280, net, bmeThp);
    if (bmeThp->getStatus() < 0) {
        // BME280 error
        info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    bhLight = new BH1750Adapter();
    lightNet = new LightSensorNet(OUTSIDE_TEMP_BH1750, net, bhLight);
    if (!bhLight->getStatus()) {
        // BH1750 error
        info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BH1750);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BH1750 error\n")));
    } else {
        lightNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_LIGHT, SENSOR_INTERVAL);
    }

    vcc = new Vcc(vccCorrection);
    vccNet = new VccNet(OUTSIDE_TEMP_VCC, net, vcc);
    vccNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_VCC, SENSOR_INTERVAL);

    // Info setup completed
    info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed\n")));
}

void loop(void) {
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

#ifdef SERIAL_DEBUG
    Serial.flush();
#endif

    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
}

