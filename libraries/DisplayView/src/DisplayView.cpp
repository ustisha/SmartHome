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