#include "DisplaySSD1289Net.h"

void DisplaySSD1289Net::receiveCommandData(Packet *p) {
    IF_SERIAL_DEBUG(printf_P(PSTR("[DisplaySSD1289Net::receiveCommandData] Cmd: %i, Data: %ld\n"),
                             p->getCommand(), p->getData()));

    if (p->getSender() == OUTSIDE_TEMP && p->getSenderPort() == PORT_BME280) {
        if (p->getCommand() == CMD_TEMPERATURE) {
            displayView->outside->temp = float(p->getData()) / 100;
        } else if (p->getCommand() == CMD_HUMIDITY) {
            displayView->outside->hum = float(p->getData()) / 100;
        } else if (p->getCommand() == CMD_PRESSURE) {
            displayView->outside->pressure = float(p->getData()) / 100;
        }
        displayView->outside->renderContent();
    }
    if (p->getSender() == COLD_CHAMBER) {
        if (p->getSenderPort() == PORT_BME280) {
            if (p->getCommand() == CMD_TEMPERATURE) {
                displayView->coldChamber->temp = float(p->getData()) / 100;
                displayView->coldChamber->renderContent();
            } else if (p->getCommand() == CMD_HUMIDITY) {
                displayView->coldChamber->hum = float(p->getData()) / 100;
                displayView->coldChamber->renderContent();
            }
        } else if (p->getSenderPort() == PORT_TEMP_CTRL) {
            if (p->getCommand() == CMD_SERVO_00) {
                uint8_t min = 18;
                uint8_t max = 155;
                displayView->coldChamber->currentProgress = lround(100 * (p->getData() - min) / (max - min));
                displayView->coldChamber->renderValve();
            }
        } else if (p->getSenderPort() == PORT_LIGHT_CTRL_00) {
            if (p->getCommand() == CMD_RELAY_00) {
                if (p->getData() == RELAY_ON) {
                    displayView->coldChamber->lamp->currentState = true;
                } else if (p->getData() == RELAY_OFF) {
                    displayView->coldChamber->lamp->currentState = false;
                }
            } else if (p->getCommand() == CMD_MODE) {
                displayView->coldChamber->lamp->currentMode = p->getData();
            }
            displayView->coldChamber->lamp->renderContent();
        }
    }
    if (p->getSender() == BATHROOM) {
        Lamp *current = nullptr;
        if (p->getSenderPort() == PORT_LIGHT_CTRL_00) {
            current = displayView->bathroom->bath;
        } else if (p->getSenderPort() == PORT_LIGHT_CTRL_01) {
            current = displayView->bathroom->toilet;
        }
        if (current != nullptr) {
            if (p->getCommand() == CMD_RELAY_00) {
                if (p->getData() == RELAY_ON) {
                    current->currentState = true;
                } else if (p->getData() == RELAY_OFF) {
                    current->currentState = false;
                }
            } else if (p->getCommand() == CMD_MODE) {
                current->currentMode = p->getData();
            } else if (p->getCommand() == CMD_TIME_LEFT) {
                current->currentTime = p->getData();
            }
            current->renderContent();
        }
    }
}
