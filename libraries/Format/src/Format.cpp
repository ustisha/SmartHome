#include "Arduino.h"
#include "Format.h"

void Format::temperature(char *formatted, float tempInput) {
    Format::temperature(formatted, tempInput, false);
}

void Format::temperature(char *formatted, float tempInput, bool c) {
    char tempString[10]{};

    if (tempInput < 0) {
        strcat(formatted, "-");
    }
    dtostrf(abs(tempInput), 3, 1, tempString);
    strcat(formatted, tempString);
    strcat(formatted, "°");
    if (c) {
        strcat(formatted, "C");
    }
}

void Format::humidity(char *formatted, float h) {
    char tempString[4]{};
    dtostrf(h, 2, 0, tempString);
    strcat(formatted, tempString);
    strcat(formatted, "%");
}

void Format::pressure(char *formatted, float hpa, uint8_t type, bool units) {
    char tempString[6]{};
    if (type == Format::PRESSURE_HPA) {
        dtostrf(hpa, 2, 1, tempString);
        strcat(formatted, tempString);
        if (units) {
            strcat(formatted, "hPa");
        }
    } else if (type == Format::PRESSURE_MMHG) {
        dtostrf((hpa / 1.33322387415), 2, 1, tempString);
        strcat(formatted, tempString);
        if (units) {
            strcat(formatted, "mmHg");
        }
    }
}

void Format::pressure(char *formatted, float hpa) {
    Format::pressure(formatted, hpa, Format::PRESSURE_MMHG, true);
}
