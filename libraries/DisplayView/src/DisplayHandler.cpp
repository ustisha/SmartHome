#include "DisplayHandler.h"
#include <DisplayView.h>

void DisplayHandler::addDisplayHandler(DisplayView *view) {
    displayView = view;
}

void DisplayHandler::render() {
    if (displayView != nullptr) {
        displayView->render();
    }
}
