#include <wiringPi.h>
#include <string.h>
#include "anslut.h"

#define RETRANSMIT 6

char *types[] = { "00", "11" };

char *units[] = { "00", "01", "10", "00" };

int pulses[] = { 250, 250, 1250, 2500, 10000 };
enum phy_signal { P_HIGH, P_ONE, P_ZERO, P_SYNC, P_PAUSE };

static void phy_send(struct device *dev, enum phy_signal sig)
{
    int pin = dev->pin;
    digitalWrite(pin, HIGH);
    delayMicroseconds(pulses[P_HIGH]);
    digitalWrite(pin, LOW);
    delayMicroseconds(pulses[sig]);
}

static void send_code(struct device *dev, char *code, unsigned int len)
{
    int i;
    for (i = 0; i < len; i++) {
        if (code[i] == '0') {
            phy_send(dev, P_ZERO);
            phy_send(dev, P_ONE);
        } else if (code[i] == '1') {
            phy_send(dev, P_ONE);
            phy_send(dev, P_ZERO);
        }
        // else panic
    }
}

static void send_packets(struct device *dev, int group, enum unit unit, int value)
{
    int i;

    for (i = 0; i < RETRANSMIT; i++) {
        phy_send(dev, P_SYNC);
        send_code(dev, dev->code, sizeof(dev->code));
        send_code(dev, group ? "0" : "1", 1);
        send_code(dev, value ? "0" : "1", 1);
        send_code(dev, dev->type, sizeof(dev->type));
        send_code(dev, units[unit], 4);
        phy_send(dev, P_PAUSE);
    }

}

void device_init(struct device *dev, int pin, const char *code, enum type type)
{
    dev->pin = pin;
    strncpy(dev->code, code, sizeof(dev->code));
    strncpy(dev->type, types[type], sizeof(dev->code));
}

void device_on(struct device *dev, enum unit unit)
{
    int group = 0;

    if (unit == U_G) {
        group = 1;
    }

    send_packets(dev, group, unit, 1);
}

void device_off(struct device *dev, enum unit unit)
{
    int group = 0;

    if (unit == U_G) {
        group = 1;
    }

    send_packets(dev, group, unit, 0);
}
