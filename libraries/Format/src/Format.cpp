#include "Arduino.h"
#include "Format.h"

void Format::temperature(char *formatted, float tempInput) {
    Format::temperature(formatted, tempInput, false);
}

void Format::temperature(char *formatted, float tempInput, bool c, const char *celsius) {
    dtostrf(tempInput, 3, 1, formatted);
    strcat(formatted, celsius);
    if (c) {
        strcat(formatted, "C");
    }
}

void Format::humidity(char *formatted, float h, uint8_t prec) {
    dtostrf(h, 2, prec, formatted);
    strcat(formatted, "%");
}

void Format::pressure(char *formatted, float hpa, uint8_t type, bool units) {
    if (type == Format::PRESSURE_HPA) {
        dtostrf(hpa, 2, 1, formatted);
        if (units) {
            strcat(formatted, "hPa");
        }
    } else if (type == Format::PRESSURE_MMHG) {
        dtostrf((hpa / 133.322387415), 2, 1, formatted);
        if (units) {
            strcat(formatted, "mmHg");
        }
    }
}

void Format::pressure(char *formatted, float hpa) {
    Format::pressure(formatted, hpa, Format::PRESSURE_MMHG, true);
}

void Format::ip(char *formatted, IPAddress *addr) {
    String ip;
    for (int i = 0; i < 4; ++i) {
        // @todo Why not working with addr[i]!?
        ip.concat(addr->operator[](i));
        if (i != 3) {
            ip.concat(".");
        }
    }
    ip.toCharArray(formatted, ip.length() + 1);
}

void Format::floatVar(char *formatted, float f) {
    String fStr(f);
    fStr.toCharArray(formatted, fStr.length() + 1);
}

void Format::strpadl(char *formatted, uint16_t val, uint8_t width, const char *pad) {
    int n = 1;
    uint16_t x = val;
    while ((x /= 10) > 0) n++;
    for (int i = 0; i < (width - n); ++i) {
        strcat(formatted, pad);
    }
    char v[8]{};
    itoa(val, v, 10);
    strcat(formatted, v);
}
