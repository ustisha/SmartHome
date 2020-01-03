//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <RF24Net.h>
#include <Ethernet.h>
#include <Format.h>
#include <Net.h>
#include <SmartNet.h>
#include <Task.h>

#ifdef SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);

    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

int freeRAM() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#endif

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 77);
IPAddress dns(192, 168, 10, 1);
IPAddress gatewayIp(192, 168, 10, 1);
EthernetClient client;
EthernetServer server(80);
RF24 radio(RF24_DEFAULT_CE, RF24_DEFAULT_CSN);
Task *ping;
SmartNet *smartNet;
RF24Net *rf24Net;

void doRequest(const char *get) {
    if (client.connect(gatewayIp, 80)) {
        client.println(get);
        client.println("Host: domain.com");
        client.println("Connection: close");
        client.println();
#ifdef SERIAL_DEBUG
        IPAddress remoteIp = client.remoteIP();
        char ipStr[16];
        Format::ip(ipStr, &remoteIp);
        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::doRequest] Remote ip: %s, Request: %s\n"), ipStr, get));
#endif
    } else {
        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::doRequest] Connection failed\n")));
    }
}

void onReceiveNRF(Packet *p) {
    char get[100];
    sprintf(get, "GET /receive/nrf24?s=%u&sp=%u&r=%u&rp=%u&cmd=%u&data=%ld HTTP/1.1",
            p->getSender(), p->getSenderPort(), p->getReceiver(), p->getReceiverPort(), p->getCommand(),
            p->getData());
    doRequest(get);
}

void onPing() {
    doRequest("GET /receive/ping");
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onPing] Ram: %d\n"), freeRAM()));
}

void onReady() {
    doRequest("GET /receive/ready");
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onReady] Ram: %d\n"), freeRAM()));
}

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] System started. Ram: %d\n"), freeRAM()));

    // Disable SD card
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    SPI.begin();
    smartNet = new SmartNet(GATEWAY, 0);
    rf24Net = new RF24Net(smartNet, GATEWAY, radio);
    rf24Net->onReceiveFunc(onReceiveNRF);

    if (Ethernet.begin(mac) == 0) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Ethernet failed to configure Ethernet using DHCP\n")));
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Ethernet shield was not found\n")));
        }
        if (Ethernet.linkStatus() == LinkOFF) {
            IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Ethernet cable is not connected\n")));
        }
        Ethernet.begin(mac, ip, dns);
    }
#ifdef SERIAL_DEBUG
    IPAddress localIp = Ethernet.localIP();
    char ipStr[16];
    Format::ip(ipStr, &localIp);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Ethernet IP: %s\n"), ipStr));
#endif

    ping = new Task(2);
    ping->each(onPing, 60000);
    ping->one(onReady, 500);

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Setup completed. Ram: %d\n"), freeRAM()));
}

int onWWWRequest(char *queryString, int resultsMaxCt) {
    int ct = 0;
    char *var;
    char *value;
    char *s = "s";
    char *sp = "sp";
    char *r = "r";
    char *rp = "rp";
    char *cmd = "cmd";
    char *data = "data";
    Packet p{};

#ifdef SERIAL_DEBUG
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onWWWRequest] Query: '%s'\n"), queryString));
#endif

    while (queryString && *queryString && ct < resultsMaxCt) {
        var = strsep(&queryString, "&");
        value = strchrnul(var, '=');
        if (*value) *value++ = '\0';
        ct++;

        if (0 == strcmp(var, s)) {
            p.setSender(atoi(value));
        } else if (0 == strcmp(var, sp)) {
            p.setSenderPort(atoi(value));
        } else if (0 == strcmp(var, r)) {
            p.setReceiver(atoi(value));
        } else if (0 == strcmp(var, rp)) {
            p.setReceiverPort(atoi(value));
        } else if (0 == strcmp(var, cmd)) {
            p.setCommand(atoi(value));
        } else if (0 == strcmp(var, data)) {
            p.setData(atoi(value));
        }
    }
    rf24Net->sendData(&p);
    return ct;
}

void requestHandler(EthernetClient *client) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::requestHandler] New client\n")));
    bool currentLineIsBlank = true;
    bool firstString = true;
    bool startQuery = false;
    bool endQuery = false;
    char buf[64] = {};
    while (client->connected()) {
        if (client->available()) {
            char c = client->read();
            if (c == ' ' && startQuery) {
                endQuery = true;
            }
            if (firstString && startQuery && !endQuery) {
                strncat(buf, &c, 1);
            }
            if (c == '\n' && currentLineIsBlank) {
                client->println("HTTP/1.1 200 OK");
                client->println("Connection: close");
                client->println();
                break;
            }
            if (c == '?' && firstString) {
                startQuery = true;
            }
            if (c == '\n') {
                currentLineIsBlank = true;
                firstString = false;
            } else if (c != '\r') {
                currentLineIsBlank = false;
            }
        }
    }
    client->flush();
    client->stop();
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::requestHandler] Client disconnected\n")));
    if (strlen(buf) > 0) {
        onWWWRequest(buf, 6);
    }
}

void loop() {
    rf24Net->tick();

    EthernetClient client = server.available();
    if (client) {
        requestHandler(&client);
    }

    ping->tick();
}
