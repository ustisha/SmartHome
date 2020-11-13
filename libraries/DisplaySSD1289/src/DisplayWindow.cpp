#include "DisplayWindow.h"

DisplayWindow::DisplayWindow(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title)
        : display(d), baseX(x), baseY(y), baseW(w), baseH(h), baseTitle(title) {
    borderColor = 0x33F6;
    bgColor = 0x963D;
    titleColor = 0xEF99;
    textColor = 0x2124;
    progressColor = 0xFDE0;
    progressBgColor = 0xE75D;
    border = 2;
    headerHeight = 15;
    padding = 3;
}

void DisplayWindow::renderAll() {
    borderRender();
    renderContent();
}

void DisplayWindow::renderContent() {
    display->setColor(bgColor);
    display->fillRoundRect(baseX + border, baseY + headerHeight, baseX + baseW - border, baseY + baseH - border);
    content();
}

void DisplayWindow::borderRender() {
    display->setColor(borderColor);
    display->fillRoundRect(baseX, baseY, baseX + baseW, baseY + baseH);
    display->setColor(titleColor);
    display->setBackColor(borderColor);
    display->setFont(mykefont2);
    display->print(baseTitle, baseX + 10, baseY + 2);
}

void DisplayWindow::progressBar(uint16_t y, uint16_t h, uint8_t progress) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[DisplayWindow::progressBar] Progress: %i\n"), progress));
    uint16_t progressBaseX = baseX + border + padding;
    uint16_t baseProgressWidth = baseW - border * 2 - padding * 2;
    uint16_t currentProgress = lround(float(progress * baseProgressWidth) / 100);
    display->setColor(progressBgColor);
    display->fillRoundRect(progressBaseX, baseY + y,progressBaseX + baseProgressWidth, baseY + y + h);
    display->setColor(progressColor);
    display->fillRoundRect(progressBaseX, baseY + y, progressBaseX + currentProgress, baseY + y + h);
}
