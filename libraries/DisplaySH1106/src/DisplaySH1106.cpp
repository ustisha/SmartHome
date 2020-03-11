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
            humInfo(64, 53, th[i]->getHumidity());
            tempInfo(64, 64, th[i]->get());
        }
        for (int i = 0; i < lIdx; ++i) {
            lightInfo(i * 64, 0, light[i], ledBitsXBM, ledOnBitsXBM);
        }
    } while (display->nextPage());
}
