#include "DisplaySH1106.h"

DisplaySH1106::DisplaySH1106(uint8_t cs, uint8_t dc, uint8_t rst, const u8g2_cb_t *rotation) {
    display = new U8G2_SH1106_128X64_NONAME_2_4W_HW_SPI(rotation, cs, dc, rst);
    display->begin();
    display->enableUTF8Print();
    display->clearDisplay();
    display->setFont(u8g2_font_nine_by_five_nbp_tf);
}

void DisplaySH1106::render() {
    display->firstPage();
    do {
        for (int i = 0; i < thIdx; ++i) {
            humInfo(84, 53, th[i]->getHumidity());
            tempInfo(84, 64, th[i]->get());
        }
        for (int i = 0; i < lIdx; ++i) {
            lightInfo(i * 64, 0, light[i], ledBitsXBM, ledOnBitsXBM);
        }
    } while (display->nextPage());
}

void DisplayView::tempInfo(const uint8_t x, const uint8_t y, float data) {
    display->setDrawColor(1);
    display->setBitmapMode(0);
    display->setCursor(x, y);
    display->print("T: ");
    display->print(data, 1);
    display->print("°C");
}

void DisplayView::humInfo(const uint8_t x, const uint8_t y, const float data) {
    display->setDrawColor(1);
    display->setBitmapMode(0);
    display->setCursor(x, y);
    display->print("H: ");
    display->print(data, 0);
    display->print("%");
}

void DisplayView::lightInfo(const uint8_t x, const uint8_t y, LightController *light, const uint8_t *ledBits, const uint8_t *ledOnBits) {
    const uint8_t statusY = y + 16;
    const uint8_t offTime = light->getOffTime();
    display->setDrawColor(1);
    display->setBitmapMode(1);
    display->drawXBMP(x + 20, y, 26, 40, ledBits);
    if (light->getRelayState() == RELAY_ON) {
        display->drawXBMP(x + 22, y + 2, 22, 15, ledOnBits);
    }
    if (light->mode == MODE_AUTO) {
        display->setCursor(x + 31, statusY);
        if (offTime || light->getRelayState() == RELAY_ON) {
            display->setDrawColor(0);
            if (offTime >= 100) {
                display->setCursor(x + 26, statusY);
            } else if (offTime >= 10) {
                display->setCursor(x + 28, statusY);
            }
            display->print(offTime);
        } else {
            display->setDrawColor(1);
            display->print("A");
        }
    } else if (light->mode == MODE_MANUAL) {
        if (light->getRelayState() == RELAY_ON) {
            display->setDrawColor(0);
            display->setCursor(x + 28, statusY);
            display->print("ON");
        } else if (light->getRelayState() == RELAY_OFF) {
            display->setDrawColor(1);
            display->setCursor(x + 24, statusY);
            display->print("OFF");
        }
    }
    if (light->energyLvl) {
        display->setCursor(x + 5, 10);
        display->setDrawColor(1);
        display->print("E");
        display->print(light->energyLvl);
    }
}