#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "anslut.h"

#define PIN 22

void setup(int sys)
{

    if (sys) {
        wiringPiSetupSys();
    } else {
        wiringPiSetupGpio();
    }
    pinMode(PIN, OUTPUT);
}

void usage(void)
{
    fprintf(stderr, "Usage: test [-n] [-s] [-p pid] [-u unit] addr val\n");
    fprintf(stderr, "-n\t- NEXA instead of ANSLUT device\n");
    fprintf(stderr, "-s\t- user sysfs GPIO instead of /dev/mem\n");
    fprintf(stderr, "-u\t- unit number, 3=group (default)\n");
    fprintf(stderr, "-p\t- data pin number (default=22)\n");
    fprintf(stderr, "addr\t- RC addr, for example 10101010101010101010101010\n");
    fprintf(stderr, "val\t- 1=on, 0=off");
}

int main(int argc, char *argv[])
{
    int opt;
    int unit = U_G, type = T_ANSLUT, pin = PIN, use_sys = 0, val;
    struct device dev;

    while ((opt = getopt(argc, argv, "nsu:p:")) != -1) {
        switch (opt) {
        case 'u':
            unit = atoi(optarg);
            break;
        case 'p':
            pin = atoi(optarg);
            break;
        case 's':
            use_sys = 1;
            break;
        case 'n':
            type = T_NEXA;
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc-1) {
        fprintf(stderr, "Not enough arguments!\n");
        usage();
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[optind]) != 26) {
        fprintf(stderr, "Address should be 26 bits long!\n");
        exit(EXIT_FAILURE);
    }

    if (unit < 0 || unit > 3) {
        fprintf(stderr, "Unit should be in range 0-3\n");
        exit(EXIT_FAILURE);
    }

    val = !!atoi(argv[optind+1]);

    printf("Setting device to %d\n", val);

    setup(use_sys);
    device_init(&dev, pin, argv[optind], type);

    if (val) {
        device_on(&dev, unit);
    } else {
        device_off(&dev, unit);
    }

    return 0 ;
}
