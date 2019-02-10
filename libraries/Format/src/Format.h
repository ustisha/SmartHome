#ifndef FORMAT_H
#define FORMAT_H


class Format {
public:
    const static uint8_t PRESSURE_HPA = 0;
    const static uint8_t PRESSURE_MMHG = 1;

    static void temperature(char *formatted, float t);
    static void temperature(char *formatted, float t, bool c);
    static void humidity(char *formatted, float h);
    static void pressure(char *formatted, float hpa);
    static void pressure(char *formatted, float hpa, uint8_t type, bool units);
};

#endif //FORMAT_H
