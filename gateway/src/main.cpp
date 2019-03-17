//#define SERIAL_DEBUG

#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <DebugLog.h>
#include <SPI.h>
#include <LoRa.h>
#include <Ethernet.h>
#include <Format.h>
#include <Net.h>

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

char gateway[] = "smarthome.ustisha.ru";
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 10, 77);
IPAddress dns(192, 168, 10, 1);
EthernetClient client;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;

void onReceive(int packetSize) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onReceive] LoRa packet. Size %d\n"), packetSize));

    if (packetSize != PACKET_SIZE) {
        return;
    }

    UInt sp(0);
    UInt rp(0);
    Long data(0);

    uint8_t sender = (uint8_t) LoRa.read();
    sp.b[0] = (uint8_t) LoRa.read();
    sp.b[1] = (uint8_t) LoRa.read();

    uint8_t receiver = (uint8_t) LoRa.read();
    rp.b[0] = (uint8_t) LoRa.read();
    rp.b[1] = (uint8_t) LoRa.read();

    uint8_t cmd = (uint8_t) LoRa.read();
    data.b[0] = (uint8_t) LoRa.read();
    data.b[1] = (uint8_t) LoRa.read();
    data.b[2] = (uint8_t) LoRa.read();
    data.b[3] = (uint8_t) LoRa.read();

    String snr(LoRa.packetSnr());
    static char snrBuf[8];
    snr.toCharArray(snrBuf, snr.length() + 1);

    IF_SERIAL_DEBUG(
            printf_P(
                    PSTR("[Gateway::onReceive] LoRa Packet. Sender: %i, Sport: %u, Receiver: %i, Rport: %u, Cmd: %i, Data: %ld\n"),
                    sender, sp.i, receiver, rp.i, cmd, data.i));

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway::onReceive] LoRa packet. RSSI: %d, SNR: %s\n"), LoRa.packetRssi(), snrBuf));

    if (client.connect(gateway, 80)) {
        IPAddress remoteIp = client.remoteIP();
        static char ipStr[16];
        Format::ip(ipStr, remoteIp);

        IF_SERIAL_DEBUG(
                printf_P(PSTR("[Gateway::onReceive] Remote ip: %s\n"), ipStr));

        static char get[128];
        static char host[32];
        sprintf(get, "GET /lora/receive?s=%i&sp=%u&r=%i&rp=%u&cmd=%i&data=%ld HTTP/1.1", sender, sp.i, receiver, rp.i, cmd, data.i);
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
        LoRa.setSpreadingFactor(7);
        LoRa.setSignalBandwidth(250E3);
        LoRa.setSyncWord(0xCC);
        LoRa.enableCrc();

        LoRa.onReceive(onReceive);
        LoRa.receive();
    } else {
        IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] LoRa failed, status: %d\n"), status));
    }

    IF_SERIAL_DEBUG(printf_P(PSTR("[Gateway] Setup completed\n")));

#ifdef SERIAL_DEBUG
    Serial.flush();
#endif
}

void loop() {

}
