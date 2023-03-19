//#define SERIAL_DEBUG

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <DebugLog.h>
#include <DebugFunc.h>
#include <RF24Net.h>
#include <Format.h>
#include <Net.h>
#include <MqttTranslate.h>
#include <SmartNet.h>
#include <Task.h>
#include <DHT.h>

const char *ssid = "";
const char *password = "";
const char *mqttServer = "";
const char *mqttUser = "";
const char *mqttPassword = "";

#define RF24_CE D2
#define RF24_CSN D4
#define DHT22_PIN D3
#define PIR D1

#define DHTTYPE DHT22

WiFiClient espClient;
PubSubClient client(espClient);
uint16_t countOk = 0;
uint16_t countFail = 0;

RF24 radio(RF24_CE, RF24_CSN);
Task *task;
SmartNet *smartNet;
RF24Net *rf24Net;
DHT *dht;
bool pirState;

void callback(char *topic, const byte *payload, unsigned int length)
{
    char payloadBuf[8]{};
    for (int i = 0; i < length; i++) {
        payloadBuf[i] = (char) (payload[i] + 0);
    }
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Message arrived: %s, '%s'\n"), topic, payloadBuf));

    Packet packet{};
    packet.setSender(GATEWAY);
    packet.setSenderPort(PORT_HTTP_HANDLER);
    char *p = strtok(topic, "/");
    uint8_t i = 0;
    int32_t r = -1, port = -1, cmd = -1;
    while (p != nullptr) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Message part: %s\n"), p));
        if (i == 1) {
            r = MqttUtils::getCode(p, components, COMPONENTS_LEN);
        } else if (i == 2) {
            port = MqttUtils::getCode(p, ports, PORTS_LEN);
        } else if (i == 3) {
            cmd = MqttUtils::getCode(p, commands, COMMANDS_LEN);
        }
        i++;
        p = strtok(nullptr, "/");
    }
    if (r == -1 || port == -1 || cmd == -1) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Can not translate topic\n")));
        return;
    }
    packet.setReceiver(r);
    packet.setReceiverPort(port);
    packet.setCommand(cmd);
    if (MqttUtils::isFloat(cmd, floatCommands, FLOAT_COMMANDS_LEN)) {
        double dd = strtod(payloadBuf, nullptr);
        packet.setData(lround(dd * 100));
    } else {
        packet.setData(strtol(payloadBuf, nullptr, 10));
    }
    bool status = rf24Net->sendData(&packet);
    rf24Net->tick();
    delay(2);

    char countMessagesBuff[16] = {};
    if (status) {
        itoa(++countOk, countMessagesBuff, 10);
        client.publish("gateway/state/rf24net/ok", countMessagesBuff);
    } else {
        itoa(++countFail, countMessagesBuff, 10);
        client.publish("gateway/state/rf24net/fail", countMessagesBuff);
    }
}

void reconnect()
{
    if (client.connected()) {
        return;
    }
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connecting to MQTT\n")));
    String clientId = "MQTT-Gateway";
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connected to MQTT: %s\n"), mqttServer));
        client.publish("gateway/state/status", "ready");
        client.publish("gateway/state/rf24net/ok", "0");
        client.publish("gateway/state/rf24net/fail", "0");
        client.subscribe("command/#");
    } else {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connecting to MQTT failed: %d\n"), client.state()));
    }
}

void onReceiveNRF(Packet *p)
{
    if (!client.connected()) {
        return;
    }
    char topic[128] = {};
    char sender[32] = {};
    char senderPort[32] = {};
    char command[32] = {};
    char payload[16] = {};
    bool cmp = MqttUtils::getName(sender, p->getSender(), components, COMPONENTS_LEN);
    bool port = MqttUtils::getName(senderPort, p->getSenderPort(), ports, PORTS_LEN);
    bool cmd = MqttUtils::getName(command, p->getCommand(), commands, COMMANDS_LEN);
    if (!cmp || !port || !cmd) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Can not translate packet\n")));
        return;
    }
    sprintf(topic, "gateway/%s/%s/%s", sender, senderPort, command);
    if (MqttUtils::isFloat(p->getCommand(), floatCommands, FLOAT_COMMANDS_LEN)) {
        double fVar = float(p->getData()) / 100;
        String fStr(fVar);
        fStr.toCharArray(payload, 16);
    } else {
        itoa(p->getData(), payload, 10);
    }
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Publish: %s, %s\n"), topic, payload));
    client.publish(topic, payload, true);
    yield();
}

void sendTemp() {
    Packet *v;
    float h = dht->readHumidity();
    float t = dht->readTemperature();
    if (isnan(h) || isnan(t)) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Failed to read from DHT sensor!\n")));
        return;
    }

    v = new Packet();
    v->setSender(WWW);
    v->setSenderPort(PORT_DHT22);
    v->setCommand(CMD_TEMPERATURE);
    v->setData((long) (t * 100));
    onReceiveNRF(v);

    v = new Packet();
    v->setSender(WWW);
    v->setSenderPort(PORT_DHT22);
    v->setCommand(CMD_HUMIDITY);
    v->setData((long) (h * 100));
    onReceiveNRF(v);
}

void pirDetection() {
    Packet *v;
    bool currentState = (bool) digitalRead(PIR);
    if (currentState != pirState) {
        pirState = currentState;
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] PIR state: %d\n"), pirState));
        v = new Packet();
        v->setSender(WWW);
        v->setSenderPort(PORT_MOTION_1);
        v->setCommand(CMD_MOTION);
        v->setData((long) pirState);
        onReceiveNRF(v);
    }
}

void setup()
{
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
#if !defined(ESP8266) && !defined(ESP32)
    printf_begin();
#endif
    IF_SERIAL_DEBUG(printf_P(PSTR("\n====== [DEBUG] ======\n")));
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] System started. Ram: %d\n"), freeRAM()));
#endif

    delay(100);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connecting to: %s "), ssid));
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
#ifdef SERIAL_DEBUG
        Serial.print(".");
        Serial.flush();
#endif
    }
    randomSeed(micros());

    ArduinoOTA.onStart([]() {
        char type[12] = {};
        if (ArduinoOTA.getCommand() == U_FLASH) {
            strcat(type, "sketch");
        } else {  // U_FS
            strcat(type, "filesystem");
        }
        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Start updating: %s\n"), type));
    });
    ArduinoOTA.onEnd([]() {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] End\n")));
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Progress: %u%%\n"), (progress / (total / 100))));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Error: %u\n"), error));
        if (error == OTA_AUTH_ERROR) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Auth Failed\n")));
        } else if (error == OTA_BEGIN_ERROR) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Begin Failed\n")));
        } else if (error == OTA_CONNECT_ERROR) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connect Failed\n")));
        } else if (error == OTA_RECEIVE_ERROR) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Receive Failed\n")));
        } else if (error == OTA_END_ERROR) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] End Failed\n")));
        }
    });
    ArduinoOTA.begin();

#ifdef SERIAL_DEBUG
    IPAddress localIp = WiFi.localIP();
    char ipStr[16];
    Format::ip(ipStr, &localIp);
    IF_SERIAL_DEBUG(printf_P(PSTR("\n[Gateway] WiFi connected, IP address: %s\n"), ipStr));
#endif

    client.setServer(mqttServer, 1883);
    client.setCallback(callback);

    smartNet = new SmartNet(GATEWAY, 0);
    rf24Net = new RF24Net(smartNet, GATEWAY, radio, RF24_PA_MAX);
    rf24Net->onReceiveFunc(onReceiveNRF);

    pinMode(DHT22_PIN, INPUT);
    dht = new DHT(DHT22_PIN, DHTTYPE);
    dht->begin();

    pinMode(PIR, INPUT);
    pirState = digitalRead(PIR);

    task = new Task(4);
    task->each(reconnect, 5000);
    task->each(sendTemp, 10000);
    task->each(pirDetection, 500);
    task->one(reconnect, 500);

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop()
{
    ArduinoOTA.handle();
    client.loop();
    rf24Net->tick();
    task->tick();
}