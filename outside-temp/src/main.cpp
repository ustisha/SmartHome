#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LoRa.h>
#include <BME280.hpp>
#include <BH1750.h>
#include <Task.h>
#include <Temperature.h>
#include <OneWireTemperature.h>
#include <TemperatureNet.h>
#include <SmartNet.h>
#include <LoraNet.h>

#define LORA_SS 53
#define LORA_RESET 9
#define LORA_DIO0 3
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
//BH1750 lm;

Task *task;
SmartNet *net;
OneWireTemperature *owTemp;
TemperatureNet *tempNet;

void setup(void) {
    Wire.begin();

    owTemp = new OneWireTemperature(&oneWire, 0);
    tempNet = new TemperatureNet(OUTSIDE_TEMP_18B20, net, owTemp);

    LoraNet ln(LORA_SS, LORA_RESET, LORA_DIO0);
    net = new SmartNet(OUTSIDE_TEMP);
    net->addRadioChannel(ln, 0);
    tempNet->addReceiver(GATEWAY, GATEWAY_HTTP_HANDLER, OUTSIDE_TEMP_CMD_TEMPERATURE, 2000);

    // Debug print
    Serial.begin(9600);


//    bme.begin(0x76);
//    Serial.println("BME sensor started");

    // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
//    lm.begin();
//    Serial.println("Light sensor started");

//    LoRa.setPins(53, 9, 3);
//    LoRa.begin(433E6);
//    LoRa.setTxPower(10);
//    LoRa.setSpreadingFactor(8);
//    LoRa.enableCrc();
//    LoRa.idle();

}

// function to print the temperature for a device
//void printTemperature(DeviceAddress deviceAddress) {
// method 1 - slower
//Serial.print("Temp C: ");
//Serial.print(sensors.getTempC(deviceAddress));
//Serial.print(" Temp F: ");
//Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

// method 2 - faster
//    float tempC = sensors.getTempC(deviceAddress);
//    if (tempC == DEVICE_DISCONNECTED_C) {
//        Serial.println("Error: Could not read temperature data");
//        return;
//    }
//    Serial.print("Temp C: ");
//    Serial.print(tempC);
//    Serial.print(" Temp F: ");
//    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
//}


/*
 * Main function. It will request the tempC from the sensors and display on Serial.
 */
void loop(void) {
    task->tick();
    net->tick();

    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
//    Serial.println("Requesting temperatures...");
//    sensors.requestTemperatures(); // Send the command to get temperatures

//    Float currentTemp;
//    Float bmeCurrentTemp;
//    Float bmeCurrentPressure;
//    Float bmeCurrentHum;

//    currentTemp.f = sensors.getTempCByIndex(0);
//    Serial.print("Temp C: ");
//    Serial.println(currentTemp.f);

//    bme.getData(&bmeCurrentTemp.f, &bmeCurrentPressure.f, &bmeCurrentHum.f);
//    Serial.print("BME Temp C: ");
//    Serial.println(bmeCurrentTemp.f);
//    Serial.print("BME Pressure mmHg: ");
//    Serial.println(bmeCurrentPressure.f);
//    Serial.print("BME Humidity %: ");
//    Serial.println(bmeCurrentHum.f);
//
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

