#define SERIAL_DEBUG

#include <avr/pgmspace.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <DebugLog.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BME280.hpp>
#include <BH1750.h>
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
//BH1750 lm;

SmartNet *net;
LoraNet *loraNet;
OneWireTemp *owTemp;
TempNet *tempNet;
BMETempHumPressure *bmeThp;
TempHumPressureNet *thpNet;

void setup(void) {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    //IF_SERIAL_DEBUG(printf_P(PSTR("%lu: MAC Received on %u %s\n\r"),millis(),pipe_num,header.toString()));
    //IF_SERIAL_DEBUG(printf_P("%u: MAC Received on %u %s\n\r",millis(),pipe_num,header->toString()));
    Wire.begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Wire started\n")));

    loraNet = new LoraNet(LORA_SS, LORA_RESET, LORA_DIO0);
    net = new SmartNet(OUTSIDE_TEMP);

    owTemp = new OneWireTemp(&oneWire, 0);
    tempNet = new TempNet(OUTSIDE_TEMP_18B20, net, owTemp);
    tempNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, 10000);

    bmeThp = new BMETempHumPressure(&bme, 0x76);
    thpNet = new TempHumPressureNet(OUTSIDE_TEMP_BME280, net, bmeThp);
    thpNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, 10000);
    thpNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_HUMIDITY, 10000);
    thpNet->addReceiver(loraNet, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_PRESSURE, 10000);

    // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
//    lm.begin();
//    Serial.println("Light sensor started");
}

/*
 * Main function. It will request the tempC from the sensors and display on Serial.
 */
void loop(void) {
    tempNet->tick();
    thpNet->tick();

//    float lux = lm.readLightLevel();
//    Serial.print("Light: ");
//    Serial.print(lux);
//    Serial.println(" lx");
//
//    Serial.println("beginPacket");
//    LoRa.beginPacket();
//
//    LoRa.write(currentTemp.b[0]);
//    LoRa.write(currentTemp.b[1]);
//    LoRa.write(currentTemp.b[2]);
//    LoRa.write(currentTemp.b[3]);
//    Serial.println("write");
//
//    LoRa.endPacket();
//    Serial.println("endPacket");
//    LoRa.receive();
//    Serial.println("receive");

//    Serial.println("DONE");

    // It responds almost immediately. Let's print out the data
//    printTemperature(insideThermometer); // Use a simple function to print out the data
//    delay(5000);
}

