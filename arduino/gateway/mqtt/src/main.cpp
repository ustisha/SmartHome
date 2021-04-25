//#define SERIAL_DEBUG

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DebugLog.h>
#include <DebugFunc.h>
#include <RF24Net.h>
#include <Format.h>
#include <Net.h>
#include <MqttTranslate.h>
#include <SmartNet.h>
#include <Task.h>

const char *ssid = "";
const char *password = "";
const char *mqttServer = "";
const char *mqttUser = "";
const char *mqttPassword = "";

#define RF24_CE 2
#define RF24_CSN 4

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE    (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

RF24 radio(RF24_CE, RF24_CSN);
Task *task;
SmartNet *smartNet;
RF24Net *rf24Net;

void callback(char *topic, byte *payload, unsigned int length)
{
    char payloadBuf[8]{};
    for (int i = 0; i < length; i++) {
        payloadBuf[i] = (char) (payload[i] + 0);
    }
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Message arrived: %s, %s\n"), topic, payloadBuf));

    Packet packet{};
    packet.setSender(GATEWAY);
    packet.setSenderPort(PORT_HTTP_HANDLER);
    char *p = strtok(topic, "/");
    uint8_t i = 0;
    while (p != nullptr) {
        if (i == 1) {
            packet.setReceiver(MqttUtils::getCode(p, components, COMPONENTS_LEN));
        } else if (i == 2) {
            packet.setReceiverPort(MqttUtils::getCode(p, ports, PORTS_LEN));
        } else if (i == 3) {
            packet.setCommand(MqttUtils::getCode(p, commands, COMMANDS_LEN));
        }
        i++;
        p = strtok(nullptr, "/");
    }
    packet.setData(atol(payloadBuf));
    rf24Net->sendData(&packet);
}

void reconnect()
{
    if (client.connected()) {
        return;
    }
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connecting to MQTT\n")));
    String clientId = "MQTT-Gateway-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connected to MQTT: %s\n"), mqttServer));
        client.publish("gateway/state", "ready");
        client.subscribe("command/#");
    } else {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Connecting to MQTT failed: %d\n"), client.state()));
    }
}

void onReceiveNRF(Packet *p)
{
    char topic[64];
    char sender[16];
    char senderPort[16];
    char command[16];
    char payload[8];
    MqttUtils::getName(sender, p->getSender(), components, COMPONENTS_LEN);
    MqttUtils::getName(senderPort, p->getSenderPort(), ports, PORTS_LEN);
    MqttUtils::getName(command, p->getCommand(), commands, COMMANDS_LEN);
    sprintf(topic, "gateway/%s/%s/%s", sender, senderPort, command);
    itoa(p->getData(), payload, 10);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Publish: %s, %s\n"), topic, payload));
    client.publish(topic, payload, true);
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

#ifdef SERIAL_DEBUG
    IPAddress localIp = WiFi.localIP();
    char ipStr[16];
    Format::ip(ipStr, &localIp);
    IF_SERIAL_DEBUG(printf_P(PSTR("\n[Gateway] WiFi connected, IP address: %s\n"), ipStr));
#endif

    client.setServer(mqttServer, 1883);
    client.setCallback(callback);

    smartNet = new SmartNet(GATEWAY, 0);
    rf24Net = new RF24Net(smartNet, GATEWAY, radio);
    rf24Net->onReceiveFunc(onReceiveNRF);

    task = new Task(2);
    task->each(reconnect, 5000);
    task->one(reconnect, 500);

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Setup completed. Ram: %d\n"), freeRAM()));
}

void loop()
{
    task->tick();
    if (client.connected()) {
        client.loop();
    }
    rf24Net->tick();
}