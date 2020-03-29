#include "DisplayView.h"

void DisplayView::addModule(THInterface *thInterface) {
    thInterface->addDisplayHandler(this);
    th[thIdx] = thInterface;
    thIdx++;
}

void DisplayView::addModule(LightController *lightController) {
    lightController->addDisplayHandler(this);
    light[lIdx] = lightController;
    lIdx++;
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

void
DisplayView::lightInfo(const uint8_t x, const uint8_t y, LightController *light, const uint8_t *ledBits, const uint8_t *ledOnBits) {
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
}
