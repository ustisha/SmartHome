//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <DHTAdapter.h>
#include <Task.h>
#include <THNet.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <DisplaySSD1289.h>
#include <DisplaySSD1289Net.h>
#include <ClockNet.h>

#ifdef SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);
    Serial.flush();
    return c;
}

void printf_begin(void) {
    fdevopen(&serial_putc, 0);
}

#endif

#ifdef SERIAL_DEBUG

int freeRAM() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#endif

#define SENSOR_INTERVAL 60

Task *task;
DHTAdapter *dht22;
THNet *thNet;
SmartNet *net;
RF24Net *rf24Net;
ClockNet *clockNet;
RF24 radio(A5, A4, 2000000);
DisplaySSD1289 *display;
DisplaySSD1289Net *displaySsd1289Net;

void onTimeRender() {
    display->clock->renderContent();
}

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    net = new SmartNet(MAIN_DISPLAY, 3);
    rf24Net = new RF24Net(net, MAIN_DISPLAY, radio);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    display = new DisplaySSD1289(38, 39, 40, 41);
    displaySsd1289Net = new DisplaySSD1289Net(net, PORT_DISPLAY, 1, display);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Display started\n")));

    clockNet = new ClockNet(net, PORT_RTC_CLOCK, 1, display->clock);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Clock started\n")));

    dht22 = new DHTAdapter(A3, DHT_TYPE_22);
    dht22->setPollInterval(SENSOR_INTERVAL);
    display->addModule(dht22);
    thNet = new THNet(net, PORT_DHT22, 2, dht22);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_TEMPERATURE, SENSOR_INTERVAL);
    thNet->addReceiver(rf24Net, GATEWAY, PORT_HTTP_HANDLER, CMD_HUMIDITY, SENSOR_INTERVAL);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] DHT22 started\n")));

    task = new Task(1);
    task->each(onTimeRender, 1000);

    // Setup completed
    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Done\n")));
}

void loop() {
    rf24Net->tick();
    thNet->tick();
    displaySsd1289Net->tick();
    clockNet->tick();
    dht22->tick();
    task->tick();
}