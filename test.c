#include <wiringPi.h>
#include "anslut.h"

int main (void)
{
    int i;
    struct device dev;
    //wiringPiSetupSys();
    wiringPiSetupGpio();
    pinMode(22, OUTPUT);

    device_init(&dev, 22, "10101010101010101010101010", T_ANSLUT);

    for(;;) {
        device_off(&dev, U_1);
        delay(1000);
        device_on(&dev, U_1);
        delay(1000);
    }

    return 0 ;
}
