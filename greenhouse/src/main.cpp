#define SERIAL_DEBUG

#include <avr/pgmspace.h>
#include <avr/power.h>
#include <LowPower.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <DebugLog.h>
#include <SPI.h>
#include <Wire.h>
#include <LoRa.h>
#include <Net.h>
#include <TInterface.h>
#include <THInterface.h>
#include <THPInterface.h>
#include <BME280Adapter.h>
#include <InfoNet.h>
#include <NetComponent.h>
#include <TNet.h>
#include <THPNet.h>
#include <SmartNet.h>
#include <LoraNetSender.h>
#include <Vcc.h>
#include <VccNet.h>
#include <Relay.h>
#include <TempController.h>

#define LORA_SS 10
#define LORA_RESET 9
#define LORA_DIO0 2

#define SENSOR_INTERVAL 20
#define POLL_INTERVAL 10

const uint16_t SLEEP = 4000;
const float vccCorrection = 1.0/1.0;

InfoNet *info;
SmartNet *net;
LoraNetSender *loraNetSender;
TNet *tempNet;
BME280Adapter *bmeThp;
THPNet *thpNet;
Vcc *vcc;
VccNet *vccNet;

void setup(void) {
    // Sec for stabilization components power
    LowPower.powerStandby(SLEEP_250MS, ADC_OFF, BOD_OFF);

#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started\n")));

    loraNetSender = new LoraNetSender(LORA_SS, LORA_RESET, LORA_DIO0);
    net = new SmartNet(GREENHOUSE);
    info = new InfoNet(PORT_INFO, net);
    // Info network started
    info->sendCommandData(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    bmeThp = new BME280Adapter(Wire, 0x76);
    bmeThp->setPollInterval(POLL_INTERVAL);
    thpNet = new THPNet(PORT_BME280, net, bmeThp);
    if (bmeThp->getStatus() < 0) {
        // BME280 error
        info->sendCommandData(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    vcc = new Vcc(vccCorrection);
    vccNet = new VccNet(PORT_VCC, net, vcc);
    vccNet->addReceiver(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_VCC, SENSOR_INTERVAL);

    // Info setup completed
    info->sendCommandData(loraNetSender, GATEWAY, GATEWAY_HTTP_HANDLER, CMD_INFO, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed\n")));
}

void loop(void) {
#ifdef SERIAL_DEBUG
    Serial.flush();
#endif

    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);

    if (bmeThp->getStatus()) {
        bmeThp->tick(SLEEP);
    }

    thpNet->tick(SLEEP);
    vccNet->tick(SLEEP);
}

