//#define SERIAL_DEBUG

#include <avr/pgmspace.h>
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
#include <BME280.hpp>
#include <Temp.h>
#include <OneWireTemp.h>
#include <SPI.h>
#include <LoRa.h>
#include <Net.h>
#include <TempNet.h>
#include <BMETempHumPressure.h>
#include <TempHumPressureNet.h>
#include <SmartNet.h>
#include <LoraNet.h>

#define LORA_SS 10
#define LORA_RESET 9
#define LORA_DIO0 3
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

SmartNet *net;
LoraNet *loraNet;
OneWireTemp *owTemp;
TempNet *tempNet;
BMETempHumPressure *bmeThp;
TempHumPressureNet *thpNet;
BHLight *bhLight;
LightNet *lightNet;

void setup(void) {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    Wire.begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Wire started\n")));

    loraNet = new LoraNet(LORA_SS, LORA_RESET, LORA_DIO0);
    net = new SmartNet(OUTSIDE_TEMP);

    owTemp = new OneWireTemp(&oneWire, 0);
    tempNet = new TempNet(OUTSIDE_TEMP_18B20, net, owTemp);
    tempNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, 2 * 60);

    bmeThp = new BMETempHumPressure(&bme, 0x76);
    thpNet = new TempHumPressureNet(OUTSIDE_TEMP_BME280, net, bmeThp);
    thpNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, 2 * 60);
    thpNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_HUMIDITY, 2 * 60);
    thpNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_PRESSURE, 2 * 60);

    bhLight = new BHLight();
    lightNet = new LightNet(OUTSIDE_TEMP_BH1750, net, bhLight);
    lightNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_LIGHT, 5 * 60);
}

void loop(void) {
    tempNet->tick();
    thpNet->tick();
    lightNet->tick();
}

