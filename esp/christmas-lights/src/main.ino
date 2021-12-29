/*
  Скетч к проекту "GyverTwink"
  - Страница проекта (схемы, описания): https://alexgyver.ru/gyvertwink/
  - Исходники на GitHub: https://github.com/AlexGyver/GyverTwink
  Проблемы с загрузкой? Читай гайд для новичков: https://alexgyver.ru/arduino-first/
  AlexGyver, AlexGyver Technologies, 2021
*/

/*
  1.1 - исправлена калибровка больше 255 светодиодов
  1.2 - исправлена ошибка с калибровкой
*/

/*
  Мигает синим - открыт портал
  Мигает жёлтым - подключаемся к точке
  Мигнул зелёным - подключился к точке
  Мигнул красным - ошибка подключения к точке
  Мигнул розовым - создал точку
*/

// ================ НАСТРОЙКИ ================
#define BTN_PIN 3      // пин кнопки
#define BTN_TOUCH 0     // 1 - сенсорная кнопка, 0 - нет

#define LED_PIN 1      // пин ленты
#define LED_TYPE WS2812 // чип ленты
#define LED_ORDER GRB   // порядок цветов ленты
#define LED_MAX 500     // макс. светодиодов

// имя точки в режиме AP
#define GT_AP_SSID "GyverTwink"
#define GT_AP_PASS "12345678"
//#define DEBUG_SERIAL_GT   // раскомментируй, чтобы включить отладку


// ================== LIBS ==================
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SimplePortal.h>
#include <FastLED.h>
#include <EEManager.h>
#include <EncButton.h>
#include "palettes.h"
#include "Timer.h"
// ================== OBJECTS ==================
WiFiServer server(80);
WiFiUDP udp;
EEManager EEwifi(portalCfg);
CRGB leds[LED_MAX];
CLEDController *strip;
EncButton<EB_TICK, BTN_PIN> btn;
IPAddress myIP;

// ================== EEPROM BLOCKS ==================
struct Cfg {
  uint16_t ledAm = 50;
  bool power = 1;
  byte bright = 100;
  bool autoCh = 0;
  bool rndCh = 0;
  byte prdCh = 1;
  bool turnOff = 0;
  byte offTmr = 60;
};
Cfg cfg;
EEManager EEcfg(cfg);

byte xy[LED_MAX][2];
EEManager EExy(xy);

struct MM {
  byte minY = 0;
  byte maxY = 255;
  byte minX = 0;
  byte maxX = 255;
  byte w = 255;
  byte h = 255;
};
MM mm;
EEManager EEmm(mm);

#define ACTIVE_PALETTES 11
struct Effects {
  bool fav = true;
  byte scale = 50;
  byte speed = 150;
};
Effects effs[ACTIVE_PALETTES * 2];
EEManager EEeff(effs);

// ================== MISC DATA ==================
Timer forceTmr(30000, false);
Timer switchTmr(0, false);
Timer offTmr(60000, false);
bool calibF = false;
byte curEff = 0;
byte forceEff = 0;

#ifdef DEBUG_SERIAL_GT
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif


void startStrip() {
    strip = &FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, LED_MAX).setCorrection(TypicalLEDStrip);
    strip->setLeds(leds, LED_MAX);
    strip->clearLedData();
    // выводим ргб
    leds[0] = CRGB::Red;
    leds[1] = CRGB::Green;
    leds[2] = CRGB::Blue;
    strip->showLeds(50);
}

bool checkButton() {
    uint32_t tmr = millis();
    while (millis() - tmr < 2000) {
        btn.tick();
        if (btn.state()) return true;
    }
    return false;
}


void fader(CRGB color) {
    static uint32_t tmr;
    static int val = 0;
    static bool dir = true;
    if (millis() - tmr >= 20) {
        tmr = millis();
        val += dir ? 3 : -3;
        val = constrain(val, 5, 120);
        if (val >= 120 || val <= 5) dir = !dir;
        strip->showColor(color, val);
    }
}

void fadeBlink(CRGB color) {
    for (int i = 0; i < 200; i += 20) {
        strip->showColor(color, i);
        delay(20);
    }
    for (int i = 200; i > 0; i -= 20) {
        strip->showColor(color, i);
        delay(20);
    }
    strip->clearLedData();
    strip->showLeds(0);
}

void portalRoutine() {
    // запускаем portal
    portalStart();
    DEBUGLN("Portal start");

    // ждём действий пользователя, мигаем
    while (!portalTick()) fader(CRGB::Blue);

    // если это 1 connect, 2 ap, 3 local, обновляем данные в епр
    if (portalStatus() <= 3) EEwifi.updateNow();

    DEBUG("Portal status: ");
    DEBUGLN(portalStatus());
}

void setupAP() {
    DEBUG("AP Mode");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(GT_AP_SSID, GT_AP_PASS);
    myIP = WiFi.softAPIP();
    server.begin();
    fadeBlink(CRGB::Magenta);
}

void setupSTA() {
    DEBUG("Connecting to AP... ");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(portalCfg.SSID, portalCfg.pass);
    uint32_t tmr = millis();
    bool state = false;
    while (millis() - tmr < 15000) {
        if (WiFi.status() == WL_CONNECTED) {
            fadeBlink(CRGB::Green);
            DEBUGLN("ok");
            myIP = WiFi.localIP();
            return;
        }
        fader(CRGB::Yellow);
        yield();
    }
    fadeBlink(CRGB::Red);
    DEBUGLN("fail");
    setupAP();
}

void switchEff() {
    // поиск favorite эффектов
    while (true) {
        if (cfg.rndCh) curEff = random(0, ACTIVE_PALETTES * 2);
        else {
            if (++curEff >= ACTIVE_PALETTES * 2) curEff = 0;
        }
        if (effs[curEff].fav) break;
    }
    DEBUG("switch to: ");
    DEBUGLN(curEff);
}

// ================== SETUP ==================
void setup() {
#ifdef DEBUG_SERIAL_GT
  Serial.begin(115200);
  DEBUGLN();
#endif
  delay(200);
  if (BTN_TOUCH) btn.setButtonLevel(HIGH);
  startStrip();
  EEPROM.begin(2048); // с запасом!

  // если это первый запуск или щелчок по кнопке, открываем портал
  if (EEwifi.begin(0, 'a') || checkButton()) portalRoutine();

  // создаём точку или подключаемся к AP
  if (portalCfg.mode == WIFI_AP || (portalCfg.mode == WIFI_STA && portalCfg.SSID[0] == '\0')) setupAP();
  else setupSTA();
  DEBUGLN(myIP);

  EEcfg.begin(EEwifi.nextAddr(), 'a');
  EEeff.begin(EEcfg.nextAddr(), 'a');
  EEmm.begin(EEeff.nextAddr(), (uint8_t)LED_MAX);
  EExy.begin(EEmm.nextAddr(), (uint8_t)LED_MAX);

  switchTmr.setPrd(cfg.prdCh * 60000ul);
  if (cfg.autoCh) switchTmr.restart();
  switchEff();
  cfg.turnOff = false;
  strip->setLeds(leds, cfg.ledAm);
  udp.begin(8888);
}

void button() {
    static bool dir = 1;

    btn.tick();
    if (btn.hasClicks(1)) {
        DEBUGLN("1x click - power");
        cfg.power = !cfg.power;
        if (!cfg.power) strip->showLeds(0);
        EEcfg.update();
    }

    if (btn.hasClicks(2)) {
        DEBUGLN("2x click - next");
        switchEff();
    }

    if (btn.step()) {
        DEBUGLN("change bright");
        if (cfg.power) {
            // меняем безопасно
            int bri = cfg.bright;
            bri += dir ? 30 : -30;
            bri = constrain(bri, 0, 255);
            cfg.bright = bri;
            if (bri == 255) {
                strip->showLeds(0);
                delay(100);
                strip->showLeds(bri);
            }
            EEcfg.update();
        }
    }

    if (btn.releaseStep()) {
        DEBUGLN("change dir");
        dir = !dir;
    }
}

void effects() {
    static Timer effTmr(30);
    static uint16_t countP = 0;
    static byte countSkip = 0;
    static byte prevEff = 255;
    static byte fadeCount = 0;

    if (effTmr.ready()) {
        byte thisEffect;

        if (forceTmr.state()) thisEffect = forceEff;
        else thisEffect = curEff;

        // эффект сменился
        if (prevEff != curEff) {
            prevEff = curEff;
            fadeCount = 25;
        }

        byte scale = effs[thisEffect].scale;
        byte speed = effs[thisEffect].speed;
        byte curPal = thisEffect;
        if (curPal >= ACTIVE_PALETTES) curPal -= ACTIVE_PALETTES;

        for (int i = 0; i < cfg.ledAm; i++) {
            byte idx;

            if (thisEffect < ACTIVE_PALETTES) {
                // первые ACTIVE_PALETTES эффектов - градиент
                // idx = map(xy[i][1], mm.minY, mm.maxY, 0, 255) + counter;   // прямой градиент
                idx = countP + ((mm.w * xy[i][0] / mm.h) + xy[i][1]) * scale / 100;   // диагональный градиент
            } else {
                // следующие - перлин нойс
                idx = inoise8(xy[i][0] * scale / 10, xy[i][1] * scale / 10, countP);
            }
            CRGB color = ColorFromPalette(paletteArr[curPal], idx, 255, LINEARBLEND);

            // плавная смена эффекта
            // меняется за 25 фреймов
            if (fadeCount) leds[i] = blend(leds[i], color, 40);
            else leds[i] = color;
        }
        if (fadeCount) fadeCount--;

        countP += (speed - 128) / 10;
        strip->showLeds(cfg.bright);
    }
}

#define MAX_UDP_PACKET 30
byte ubuf[MAX_UDP_PACKET];

void reply(byte* data, byte size) {
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write("GT");
    udp.write(data, size);
    udp.endPacket();
}

void parsing() {
    static uint32_t tmr;
    if (udp.parsePacket()) {
        if (millis() - tmr < 50) {  // "антидребезг" приёма
            udp.read(ubuf, MAX_UDP_PACKET);
            return;
        }
        tmr = millis();
        int n = udp.read(ubuf, MAX_UDP_PACKET);
        ubuf[n] = 0;
        if (ubuf[0] != 'G' || ubuf[1] != 'T') return;
        /*for (int i = 2; i < n; i++) {
          DEBUG(ubuf[i]);
          DEBUG(',');
          }
          DEBUGLN();*/
        byte answ[10];

        switch (ubuf[2]) {
            case 0:   // запрос IP
                delay(myIP[3] * 2);
                answ[0] = 0;
                answ[1] = myIP[3];
                reply(answ, 2);
                break;

            case 1:   // запрос настроек
                answ[0] = 1;
                answ[1] = cfg.ledAm / 100;
                answ[2] = cfg.ledAm % 100;
                answ[3] = cfg.power;
                answ[4] = cfg.bright;
                answ[5] = cfg.autoCh;
                answ[6] = cfg.rndCh;
                answ[7] = cfg.prdCh;
                answ[8] = cfg.turnOff;
                answ[9] = cfg.offTmr;
                reply(answ, 10);
                break;

            case 2:   // приём настроек
                forceTmr.stop();
                switch (ubuf[3]) {
                    case 0: cfg.ledAm = ubuf[4] * 100 + ubuf[5];
                        strip->setLeds(leds, cfg.ledAm);
                        break;
                    case 1: cfg.power = ubuf[4];
                        break;
                    case 2: cfg.bright = ubuf[4];
                        break;
                    case 3: cfg.autoCh = ubuf[4];
                        if (cfg.autoCh) switchTmr.restart();
                        else switchTmr.stop();
                        break;
                    case 4: cfg.rndCh = ubuf[4];
                        break;
                    case 5: cfg.prdCh = ubuf[4];
                        switchTmr.setPrd(cfg.prdCh * 60000ul);
                        if (cfg.autoCh) switchTmr.restart();
                        break;
                    case 6:   // нехт эффект
                        switchEff();
                        if (cfg.autoCh) switchTmr.restart();
                        return;
                        break;
                    case 7:
                        cfg.turnOff = ubuf[4];
                        if (cfg.turnOff) offTmr.restart();
                        else offTmr.stop();
                        break;
                    case 8:
                        cfg.offTmr = ubuf[4];
                        offTmr.setPrd(cfg.offTmr * 60000ul);
                        if (cfg.turnOff) offTmr.restart();
                        break;
                }
                if (!cfg.power) strip->showLeds(0);
                EEcfg.update();
                break;

            case 3:
                switch (ubuf[3]) {
                    case 0:   // запуск калибровки
                        DEBUGLN("Calibration start");
                        calibF = true;
                        strip->clearLedData();
                        strip->showLeds(0);
                        break;

                    case 1:   // следующий лед
                    {
                        int curLed = ubuf[4] * 100 + ubuf[5];
                        if (curLed > 0) {
                            xy[curLed - 1][0] = ubuf[6];
                            xy[curLed - 1][1] = ubuf[7];
                        }
                        strip->clearLedData();
                        leds[curLed] = CRGB::White;
                        strip->showLeds(200);
                    }
                        break;

                    case 2:   // калибровка окончена
                        DEBUGLN("Finished");
                        calibF = false;
                        strip->clearLedData();
                        strip->showLeds(0);
                        EExy.updateNow();

                        mm.minY = 255;
                        mm.maxY = 0;
                        mm.minX = 255;
                        mm.maxX = 0;
                        for (int i = 0; i < cfg.ledAm; i++) {
                            mm.minX = min(mm.minX, xy[i][0]);
                            mm.maxX = max(mm.maxX, xy[i][0]);
                            mm.minY = min(mm.minY, xy[i][1]);
                            mm.maxY = max(mm.maxY, xy[i][1]);
                        }
                        mm.w = mm.maxX - mm.minX;
                        mm.h = mm.maxY - mm.minY;
                        EEmm.updateNow();
                        break;
                }
                break;

            case 4:   // управление эффектами
                forceTmr.restart();
                EEeff.update();
                switch (ubuf[3]) {
                    case 0:   // выбор эффекта в дропе
                        forceEff = ubuf[4];
                        answ[0] = 4;
                        answ[1] = effs[forceEff].fav;
                        answ[2] = effs[forceEff].scale;
                        answ[3] = effs[forceEff].speed;
                        reply(answ, 4);
                        break;
                    case 1:   // флажок избранное
                        effs[forceEff].fav = ubuf[4];
                        break;
                    case 2:   // масштаб
                        effs[forceEff].scale = ubuf[4];
                        break;
                    case 3:   // скорость
                        effs[forceEff].speed = ubuf[4];
                        break;
                }
                break;
        }
    }
}

// ================== LOOP ==================
void loop() {
  button();   // опрос кнопки

  // менеджер епром
  EEcfg.tick();
  EEeff.tick();

  parsing();  // парсим udp

  // таймер принудительного показа эффектов
  if (forceTmr.ready()) {
    forceTmr.stop();
    switchEff();
  }

  // форс выключен и настало время менять эффект
  if (!forceTmr.state() && switchTmr.ready()) switchEff();

  // таймер выключения
  if (offTmr.ready()) {
    offTmr.stop();
    cfg.turnOff = false;
    cfg.power = false;
    strip->showLeds(0);
    EEcfg.update();
    DEBUGLN("Off tmr");
  }

  // показываем эффект, если включены
  if (!calibF && cfg.power) effects();
}
