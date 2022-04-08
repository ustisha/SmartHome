//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <SmartNet.h>
#include <RF24Net.h>
#include <EEPROMex.h>
#include <ServoEasing.hpp>
#include <CtrlServo.h>
#include <CtrlServoNet.h>

#ifdef SERIAL_DEBUG

int serial_putc(char c, FILE *) {
    Serial.write(c);
    Serial.flush();
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

#define SERVO1 5
#define SERVO2 6
#define SERVO3 2

CtrlServo *servo1, *servo2, *servo3;
CtrlServoNet *servoNet1, *servoNet2, *servoNet3;
SmartNet *net;
RF24Net *rf24Net;
RF24 radio(RF24_DEFAULT_CE, RF24_DEFAULT_CSN);

void setup(void) {
#ifdef SERIAL_DEBUG
    #if defined(__AVR_ATmega4808__)
    delay(2000);
#endif
    Serial.begin(57600);
    while (!Serial);
    printf_begin();
    IF_SERIAL_DEBUG(printf_P(PSTR("====== [DEBUG] ======\n")));
#endif

    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] System started. Ram: %d\n"), freeRAM()));

    EEPROM.setMemPool(0, EEPROMSizeATmega328);

    net = new SmartNet(BATHROOM_VALVE, 4);
    rf24Net = new RF24Net(net, BATHROOM_VALVE, radio, RF24_PA_MIN);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Radio initialized\n")));

    // Info network started
    net->sendInfo(rf24Net, INFO_NETWORK_STARTED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Network started\n")));

    servo1 = new CtrlServo(SERVO1, 3, 80);
    servo2 = new CtrlServo(SERVO2, 8, 80);
    servo3 = new CtrlServo(SERVO3, 80, 0);
    servoNet1 = new CtrlServoNet(net, PORT_SERVO_00, 1, servo1);
    servoNet2 = new CtrlServoNet(net, PORT_SERVO_01, 1, servo2);
    servoNet3 = new CtrlServoNet(net, PORT_SERVO_02, 1, servo3);
    servo1->addNet(rf24Net, servoNet1, GATEWAY, PORT_HTTP_HANDLER);
    servo2->addNet(rf24Net, servoNet2, GATEWAY, PORT_HTTP_HANDLER);
    servo3->addNet(rf24Net, servoNet3, GATEWAY, PORT_HTTP_HANDLER);

    if (servo1->getStatus() == INVALID_SERVO) {
        net->sendInfo(rf24Net, INFO_ERROR_SERVO_00);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Servo 1 error\n")));

    }

    if (servo2->getStatus() == INVALID_SERVO) {
        net->sendInfo(rf24Net, INFO_ERROR_SERVO_01);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Servo 2 error\n")));

    }

    if (servo3->getStatus() == INVALID_SERVO) {
        net->sendInfo(rf24Net, INFO_ERROR_SERVO_03);
        IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Servo 3 error\n")));

    }

    net->sendInfo(rf24Net, INFO_SETUP_COMPLETED);
    IF_SERIAL_DEBUG(printf_P(PSTR("[Main] Setup completed. Ram: %d\n"), freeRAM()));
    servo1->sendValues();
    servo2->sendValues();
    servo3->sendValues();
}

void loop(void) {
#ifdef SERIAL_DEBUG
    Serial.flush();
    //net->tick();
#endif
    rf24Net->tick();
    servo1->tick();
    servo2->tick();
    servo3->tick();
}

