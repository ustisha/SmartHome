//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <LightSensorNet.h>
#include <BME280Adapter.h>
#include <InfoNet.h>
#include <TNet.h>
#include <THPNet.h>
#include <SmartNet.h>
#include <LoraNetSender.h>
#include <Vcc.h>
#include <VccNet.h>
#include <Relay.h>
#include <TempController.h>
#include <TempControllerNet.h>

#ifdef IF_SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);

    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

#endif

#define LORA_SS 10
#define LORA_RESET 9
#define LORA_DIO0 2

#define R1 4
#define R2 6
#define SERV1 5
#define SERV2 7

#define SENSOR_INTERVAL 120
#define POLL_INTERVAL 110

const float vccCorrection = 1.0 / 1.0;

InfoNet *info;
SmartNet *net;
LoraNetSender *loraNetSender;
BME280Adapter *bmeThp;
THPNet *thpNet;
Vcc *vcc;
VccNet *vccNet;
Relay *r1, *r2;
TempController *tempController;
TempControllerNet *tempControllerNet;
Servo *serv1, *serv2;

void onReceive(int packetSize) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main::onReceive] LoRa packet. Size %d\n"), packetSize));

    if (packetSize != PACKET_SIZE) {
        return;
    }

    UInt sp(0);
    UInt rp(0);
    Long data(0);
    Packet p{};

    p.sender = (uint8_t) LoRa.read();
    sp.b[0] = (uint8_t) LoRa.read();
    sp.b[1] = (uint8_t) LoRa.read();
    p.sp = sp.i;

    p.receiver = (uint8_t) LoRa.read();
    if (p.receiver != GREENHOUSE) {
        return;
    }
    rp.b[0] = (uint8_t) LoRa.read();
    rp.b[1] = (uint8_t) LoRa.read();
    p.rp = rp.i;

    p.cmd = (uint8_t) LoRa.read();
    data.b[0] = (uint8_t) LoRa.read();
    data.b[1] = (uint8_t) LoRa.read();
    data.b[2] = (uint8_t) LoRa.read();
    data.b[3] = (uint8_t) LoRa.read();
    p.data = data.i;

    net->commandReceived(p);
}

void setup(void) {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started\n")));

    loraNetSender = new LoraNetSender(LORA_SS, LORA_RESET, LORA_DIO0);
    loraNetSender->onReciveFunc(onReceive);
    net = new SmartNet(GREENHOUSE);
    info = new InfoNet(PORT_INFO, net);
    // Info network started
    info->sendCommandData(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    bmeThp = new BME280Adapter(Wire, 0x76);
    bmeThp->setPollInterval(POLL_INTERVAL);
    thpNet = new THPNet(PORT_BME280, net, bmeThp);
    if (bmeThp->getStatus() < 0) {
        // BME280 error
        info->sendCommandData(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_ERROR_BME280);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] BME280 error\n")));
    } else {
        thpNet->addReceiver(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
        thpNet->addReceiver(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_PRESSURE, SENSOR_INTERVAL);
    }

    r1 = new Relay(R1);
    r2 = new Relay(R2);
    serv1 = new Servo();
    serv1->attach(SERV1);
    serv2 = new Servo();
    serv2->attach(SERV2);
    tempController = new TempController(bmeThp, 26.0, 30.0);
    tempController->setTimeout(2);
    tempController->addRelay(r1, 0, true);
    tempController->addRelay(r2, 1, true, 0.5, 0.1);
    tempController->addServo(serv1, 0, false, 160, 1.4);
    tempController->addServo(serv2, 1, false, 160, 1.8);
    tempControllerNet = new TempControllerNet(PORT_TEMP_CONTROLLER, net, tempController);
    tempController->addNet(loraNetSender, tempControllerNet, GATEWAY, PORT_HTTP_HANDLER);

    vcc = new Vcc(vccCorrection);
    vccNet = new VccNet(PORT_VCC, net, vcc);
    vccNet->addReceiver(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_VCC, SENSOR_INTERVAL);

    // Info setup completed
    info->sendCommandData(loraNetSender, GATEWAY, PORT_HTTP_HANDLER, CMD_INFO, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed\n")));
}

void loop(void) {
#ifdef SERIAL_DEBUG
    Serial.flush();
#endif
    if (bmeThp->getStatus()) {
        bmeThp->tick();
    }

    thpNet->tick();
    vccNet->tick();
    tempController->tick();
    tempControllerNet->tick();
}

