#include <stdio.h>
#include "io.h"


int cfg_pin(int pin, int output, int val)
{
    if (output)
        fprintf(stderr, "cfg p%d, output, %d\n", pin, val);
    else
        fprintf(stderr, "cfg p%d, input, %s\n",
            pin, val ? "pullup" : "no pullup");

    return 1;
}

int set_pin(int pin, int val)
{
    fprintf(stderr, "set p%d -> %d\n", pin, val);
    return val;
}

int get_pin(int pin)
{
    fprintf(stderr, "get p%d <- 0\n", pin);
    return pin;
}

int pwm(int duty, int length)
{
    return 0;
}

int wait(int ms)
{
    return 0;
}

void load_from_eeprom(void)
{
}

void save_to_eeprom(void)
{
}
