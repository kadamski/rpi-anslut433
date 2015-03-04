#ifndef _ANSLUTLIB_H_
#define _ANSLUTLIB_H_

enum type { T_ANSLUT, T_NEXA };
enum unit { U_0, U_1, U_2, U_G };

struct device
{
    unsigned int pin;
    char code[26];
    char type[2];
};

void device_init(struct device *dev, int pin, const char *code, enum type type);
void device_on(struct device *dev, enum unit unit);
void device_off(struct device *dev, enum unit unit);
#endif
