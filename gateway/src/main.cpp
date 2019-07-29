//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <LoraNetReceive.h>
#include <Ethernet.h>
#include <Format.h>
#include <Net.h>
#include <Task.h>

#ifdef IF_SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);

    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

#endif

const char domain[] PROGMEM = "home.ustisha.ru";
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 77);
IPAddress dns(192, 168, 10, 1);
IPAddress gateway(192, 168, 10, 1);
EthernetClient client;
EthernetServer server(80);
Task *ping;
LoraNetReceive *loraNet;

void sendData(Packet *p) {
    IF_SERIAL_DEBUG(
            printf_P(
                    PSTR("[Gateway::sendData] LoRa Packet. Sender: %i, Sport: %u, Receiver: %i, Rport: %u, Cmd: %i, Data: %ld\n"),
                    p->sender, p->sp, p->receiver, p->rp, p->cmd, p->data));

    if (client.connect(gateway, 80)) {
        IPAddress remoteIp = client.remoteIP();
        static char ipStr[16];
        Format::ip(ipStr, remoteIp);

        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::sendData] Remote ip: %s\n"), ipStr));

        static char get[128];
        static char host[32];
        sprintf(get, "GET /lora/receive?s=%i&sp=%u&r=%i&rp=%u&cmd=%i&data=%ld HTTP/1.1",
                p->sender, p->sp, p->receiver, p->rp, p->cmd, p->data);
        client.println(get);
        sprintf(host, "Host: %s", domain);
        client.println(host);
        client.println("Connection: close");
        client.println();
        client.flush();
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::sendData] Client stop\n")));
    } else {
        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::sendData] Connection failed\n")));
    }
}

void onReceive(int packetSize) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onReceive] LoRa packet. Size %d\n"), packetSize));

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
    rp.b[0] = (uint8_t) LoRa.read();
    rp.b[1] = (uint8_t) LoRa.read();
    p.rp = rp.i;

    p.cmd = (uint8_t) LoRa.read();
    data.b[0] = (uint8_t) LoRa.read();
    data.b[1] = (uint8_t) LoRa.read();
    data.b[2] = (uint8_t) LoRa.read();
    data.b[3] = (uint8_t) LoRa.read();
    p.data = data.i;

    sendData(&p);
}

void onPing() {
    Packet p{};
    p.sender = GATEWAY;
    p.sp = PORT_INFO;
    p.receiver = WWW;
    p.rp = PORT_INFO;
    p.cmd = CMD_INFO;
    p.data = INFO_PING;
    sendData(&p);
}

void onCompleted() {
    Packet p{};
    p.sender = GATEWAY;
    p.sp = PORT_INFO;
    p.receiver = WWW;
    p.rp = PORT_INFO;
    p.cmd = CMD_INFO;
    p.data = INFO_SETUP_COMPLETED;
    sendData(&p);
}

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] System started\n")));

    // Disable SD card
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

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
    static char ipStr[16];
    Format::ip(ipStr, localIp);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Ethernet IP: %s\n"), ipStr));
#endif

    loraNet = new LoraNetReceive(6, 5, 3);
    loraNet->onReceiveFunc(onReceive);

    ping = new Task();
    ping->each(onPing, 60000);
    ping->one(onCompleted, 1000);

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Setup completed\n")));

#ifdef SERIAL_DEBUG
    Serial.flush();
#endif
}

int onWWWRequest(char *queryString, int resultsMaxCt) {
    int ct = 0;
    char *var, *value;
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
            p.sender = atoi(value);
        } else if (0 == strcmp(var, sp)) {
            p.sp = atoi(value);
        } else if (0 == strcmp(var, r)) {
            p.receiver = atoi(value);
        } else if (0 == strcmp(var, rp)) {
            p.rp = atoi(value);
        } else if (0 == strcmp(var, cmd)) {
            p.cmd = atoi(value);
        } else if (0 == strcmp(var, data)) {
            p.data = atoi(value);
        }
    }
    loraNet->sendData(p);
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
                client->println(PSTR("HTTP/1.1 200 OK"));
                client->println(PSTR("Connection: close"));
                client->println();
                break;
            }
            if (c == '?' && firstString) {
                startQuery = true;
            }
            if (c == '\n') {
                currentLineIsBlank = true;
                firstString = false;
            }
            else if (c != '\r') {
                currentLineIsBlank = false;
            }
        }
    }
    client->flush();
    client->stop();
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::requestHandler] Client disconnected\n")));
    onWWWRequest(buf, 6);
}

void loop() {
    ping->tick();

    EthernetClient client = server.available();
    if (client) {
        requestHandler(&client);
    }
}
