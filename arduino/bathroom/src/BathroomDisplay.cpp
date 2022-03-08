#include "BathroomDisplay.h"

void BathroomDisplay::render()
{
    const uint8_t blockWidth = 76;
    for (int i = 0; i < thIdx; ++i) {
        if (!thUpdate[i].rendered) {
            renderTempIcon(blockWidth + 8, 64);
            renderHumIcon(blockWidth + 42, 64);
            thUpdate[i].rendered = true;
        }
        if (thUpdate[i].value != th[i]->get()) {
            tempUpdate(blockWidth + 10, 64, th[i]->get());
            thUpdate[i].value = th[i]->get();
        }
        if (thUpdate[i].value2 != th[i]->getHumidity()) {
            humUpdate(blockWidth + 42, 64, th[i]->getHumidity());
            thUpdate[i].value2 = th[i]->getHumidity();
        }
        if (!washingUpdate[i].rendered) {
            renderWashingMachineIcon(blockWidth + 8, 85);
            washingUpdate[i].rendered = true;
        }
        if (washingUpdate[i].remainingSec != washing->remainingSec
            || washingUpdate[i].value != washing->state
            || washingUpdate[i].opened != washing->doorOpened) {
            tft->fillRect(blockWidth + 26, 92, 50, 8, 0x000);
            tft->setCursor(blockWidth + 26, 92);
            switch (washing->state) {
                case WashingMachine::READY:
                    if (washing->doorOpened) {
                        tft->setTextColor(0xF750, 0x000);
                        tft->print("OPENED");
                    } else {
                        tft->setTextColor(0x5D4C, 0x000);
                        tft->print("CLOSED");
                    }
                    break;
                case WashingMachine::RUN:
                    tft->setTextColor(0xF628, 0x000);
                    {
                        const uint8_t hours = floor((double) washing->remainingSec / 3600);
                        tft->print(hours);
                        tft->print(":");
                        const uint8_t sec = floor((double) (washing->remainingSec - hours * 3600) / 60);
                        if (sec < 10) {
                            tft->print("0");
                        }
                        tft->print(sec);
                    }
                    break;
                case WashingMachine::FINISHED:
                    tft->setTextColor(0x5D4C, 0x000);
                    tft->print("FINISHED");
                    break;
                case WashingMachine::ERR:
                    tft->setTextColor(0xE9E6, 0x000);
                    tft->print("!ERROR!");
                    break;
            }
            washingUpdate[i].remainingSec = washing->remainingSec;
            washingUpdate[i].opened = washing->doorOpened;
            washingUpdate[i].value = washing->state;
        }
    }

    for (int i = 0; i < lIdx; ++i) {
        const uint8_t blockStart = i * blockWidth;
        const uint8_t blockHeight = 124;
        const uint8_t space = 1 + 2 * i;

        // background
        tft->drawRoundRect(blockStart + space, 1, blockWidth, blockHeight, 4, 0x073E);
        tft->drawRoundRect(blockStart + space + 1, 2, blockWidth, blockHeight, 4, 0x0536);
        // light control
        lightInfo(blockStart + space, 1, light[i], &lightUpdate[i]);
    }

    // vent percent
    for (int i = 0; i < valueIdx; ++i) {
        const uint8_t blockStart = i * blockWidth;
        const uint8_t space = 1 + 2 * i;
        if (valueUpdate[i].value != valueInt[i]->get()) {
            renderScale(blockStart + space + 50, 7, valueInt[i]->get());
            valueUpdate[i].value = valueInt[i]->get();
        }
    }

    // floorTemp temp
    for (int i = 0; i < floorTempIdx; ++i) {
        const uint8_t blockStart = i * blockWidth;
        const uint8_t space = 1 + 2 * i;
        if (!floorUpdate[i].rendered) {
            renderHeaterIcon(blockStart + space + 5, 105);
            floorUpdate[i].rendered = true;
        }
        if (floorTemp[i]->getStatus() > 0) {
            if (floorUpdate[i].value != floorTemp[i]->get()) {
                tempUpdate(blockStart + space + 12, 108, floorTemp[i]->get(), 1);
                floorUpdate[i].value = floorTemp[i]->get();
            }
        }
    }
}

