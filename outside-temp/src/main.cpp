//#define SERIAL_DEBUG

#include <avr/pgmspace.h>
#include <avr/power.h>
#include <LowPower.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <DebugLog.h>
#include <Wire.h>
#include <LightSensor.h>
#include <LightNet.h>
#include <BHLight.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BME280.h>
#include <Temp.h>
#include <OneWireTemp.h>
#include <SPI.h>
#include <LoRa.h>
#include <Net.h>
#include <NetInfo.h>
#include <TempNet.h>
#include <BMETempHumPressure.h>
#include <TempHumPressureNet.h>
#include <SmartNet.h>
#include <LoraNetEE.h>

#define LORA_SS 10
#define LORA_RESET 9
#define LORA_DIO0 3
#define ONE_WIRE_BUS 2

#define SENSOR_INTERVAL 10

OneWire oneWire(ONE_WIRE_BUS);

NetInfo *info;
SmartNet *net;
LoraNetEE *loraNetEE;
OneWireTemp *owTemp;
TempNet *tempNet;
BMETempHumPressure *bmeThp;
TempHumPressureNet *thpNet;
BHLight *bhLight;
LightNet *lightNet;

void setup(void) {
    // Sec for stabilization components power
    LowPower.powerStandby(SLEEP_250MS, ADC_OFF, BOD_OFF);

#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#else
    // Only in production mode. Serial print not working with this settings.
    //clock_prescale_set(clock_div_16);
#endif

    Wire.begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Wire started\n")));

    loraNetEE = new LoraNetEE(LORA_SS, LORA_RESET, LORA_DIO0);
    net = new SmartNet(OUTSIDE_TEMP);
    info = new NetInfo(OUTSIDE_TEMP_INFO, net);
    // Info network started
    info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    owTemp = new OneWireTemp(&oneWire, 0);
    tempNet = new TempNet(OUTSIDE_TEMP_18B20, net, owTemp);
    tempNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);

    bmeThp = new BMETempHumPressure(Wire, 0x76);
    thpNet = new TempHumPressureNet(OUTSIDE_TEMP_BME280, net, bmeThp);
    if (bmeThp->getStatus() < 0) {
        // BME280 error
        info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    bhLight = new BHLight();
    lightNet = new LightNet(OUTSIDE_TEMP_BH1750, net, bhLight);
    if (!bhLight->getStatus()) {
        // BH1750 error
        info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BH1750);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BH1750 error\n")));
    } else {
        lightNet->addReceiver(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_LIGHT, SENSOR_INTERVAL);
    }

    // Info setup completed
    info->sendCommandData(loraNetEE, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed\n")));
}

void loop(void) {
    tempNet->tick(4000);
    thpNet->tick(4000);
    lightNet->tick(4000);

#ifdef SERIAL_DEBUG
    Serial.flush();
#endif

    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
}

