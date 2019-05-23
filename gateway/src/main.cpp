//#define SERIAL_DEBUG

#include <DebugLog.h>
#include <SPI.h>
#include <LoRa.h>
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

union UInt {
    uint16_t i = 0;
    uint8_t b[sizeof(uint16_t)];

    UInt(uint16_t value) : i(value) {

    };
};

union Long {
    long i = 0;
    uint8_t b[sizeof(long)];

    Long(long value) : i(value) {

    };
};

char gateway[] = "home.ustisha.ru";
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 77);
IPAddress dns(192, 168, 10, 1);
EthernetClient client;
EthernetServer server(80);
Task *ping;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;

void sendData(Packet *p, int rssi = 0, float snr = 0) {
    String snrStr(snr);
    static char snrBuf[8];
    snrStr.toCharArray(snrBuf, snrStr.length() + 1);

    IF_SERIAL_DEBUG(
            printf_P(
                    PSTR("[Gateway::onReceive] LoRa Packet. Sender: %i, Sport: %u, Receiver: %i, Rport: %u, Cmd: %i, Data: %ld\n"),
                    p->sender, p->sp, p->receiver, p->rp, p->cmd, p->data));

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onReceive] LoRa packet. RSSI: %d, SNR: %s\n"),
                             rssi, snrBuf));

    if (client.connect(gateway, 80)) {
        IPAddress remoteIp = client.remoteIP();
        static char ipStr[16];
        Format::ip(ipStr, remoteIp);

        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::onReceive] Remote ip: %s\n"), ipStr));

        static char get[128];
        static char host[32];
        sprintf(get, "GET /lora/receive?s=%i&sp=%u&r=%i&rp=%u&cmd=%i&data=%ld&rssi=%d&snr=%s HTTP/1.1",
                p->sender, p->sp, p->receiver, p->rp, p->cmd, p->data, rssi, snrBuf);
        client.println(get);
        sprintf(host, "Host: %s", gateway);
        client.println(host);
        client.println("Connection: close");
        client.println();
        client.flush();
    } else {
        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::onReceive] Connection failed\n")));
    }

#ifdef SERIAL_DEBUG
    Serial.flush();
#endif
}

void onReceive(int packetSize) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onReceive] LoRa packet. Size %d\n"), packetSize));

    if (packetSize != PACKET_SIZE) {
        return;
    }

    UInt sp(0);
    UInt rp(0);
    Long data(0);
    Packet p;

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

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    sendData(&p, rssi, snr);
}

void onPing() {
    Packet p;
    p.sender = GATEWAY;
    p.sp = PORT_INFO;
    p.receiver = WWW;
    p.rp = PORT_INFO;
    p.cmd = CMD_INFO;
    p.data = INFO_PING;
    sendData(&p);
}

void onCompleted() {
    Packet p;
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

    LoRa.setPins(6, 5, 3);
    int status = LoRa.begin(510E6);
    if (status) {
        LoRa.setSpreadingFactor(10);
        LoRa.setSignalBandwidth(250E3);
        LoRa.setSyncWord(0xCC);
        LoRa.enableCrc();

        LoRa.onReceive(onReceive);
        LoRa.receive();
    } else {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] LoRa failed, status: %d\n"), status));
    }

    ping = new Task();
    ping->each(onPing, 60000);
    ping->one(onCompleted, 1000);

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Setup completed\n")));

#ifdef SERIAL_DEBUG
    Serial.flush();
#endif
}

void loop() {
    ping->tick();

    /*EthernetClient client = server.available();
    if (client) {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::loop] New client\n")));
        Serial.println("========");
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                // если мы получили символ новой строки
                // и текущая строка пустая, значит запрос окончен
                // и можно отослать ответ
                if (c == '\n' && currentLineIsBlank) {
                    // отсылаем старндартные http заголовки
                    client.println("HTTP/1.1 200 OK");
                    client.println("Connection: close");  // соединение будет закрыто после получения ответа
                    client.println();
                    break;
                }
                if (c == '\n') {
                    // у нас новая строка
                    currentLineIsBlank = true;
                }
                else if (c != '\r') {
                    // у нас символ на текущей строке
                    currentLineIsBlank = false;
                }
            }
        }
        // даем браузеру время получить данные
        delay(1);
        // закрываем соединение
        client.stop();
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::loop] Client disconnected\n")));
    }*/
}
