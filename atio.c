#include <stdbool.h>
#include <avr/io.h>
#include "atio.h"


struct pin_def {
    int port;
    int bit_num;
};

#define NUM_GPIO_PINS 8

static const struct pin_def pin_defs[] = {
    { 2, 0 },
    { 2, 1 },
    { 2, 2 },
    { 1, 4 },
    { 1, 3 },
    { 1, 2 },
    { 1, 1 },
    { 1, 0 },
};


// use a big switch case to ensure I/O is performed in a single instruction
// as long as sfr is constant
#define set_bit_val(sfr, bit, val) \
    switch (bit) { \
    case 0: _set_bit_val(sfr, 0, val); break; \
    case 1: _set_bit_val(sfr, 1, val); break; \
    case 2: _set_bit_val(sfr, 2, val); break; \
    case 3: _set_bit_val(sfr, 3, val); break; \
    case 4: _set_bit_val(sfr, 4, val); break; \
    case 5: _set_bit_val(sfr, 5, val); break; \
    case 6: _set_bit_val(sfr, 6, val); break; \
    case 7: \
    default: _set_bit_val(sfr, 7, val); break; \
    }


#define set_port_bit(base_name, port_num, bit, val) \
    do { \
        if (port_num == 1) { \
            set_bit_val(base_name ## A, bit, val); \
        } else { \
            set_bit_val(base_name ## B, bit, val); \
        } \
    } while (0)


int cfg_pin(int pin, int output, int val)
{
    // big switch-case so array access gets inlined, and binary is a few
    // hundred bytes smaller
    switch (pin) {
    case 1:
        set_port_bit(DDR, pin_defs[0].port, pin_defs[0].bit_num, output);
        set_port_bit(PORT, pin_defs[0].port, pin_defs[0].bit_num, val);
        break;

    case 2:
        set_port_bit(DDR, pin_defs[1].port, pin_defs[1].bit_num, output);
        set_port_bit(PORT, pin_defs[1].port, pin_defs[1].bit_num, val);
        break;

    case 3:
        set_port_bit(DDR, pin_defs[2].port, pin_defs[2].bit_num, output);
        set_port_bit(PORT, pin_defs[2].port, pin_defs[2].bit_num, val);
        break;

    case 4:
        set_port_bit(DDR, pin_defs[3].port, pin_defs[3].bit_num, output);
        set_port_bit(PORT, pin_defs[3].port, pin_defs[3].bit_num, val);
        break;

    case 5:
        set_port_bit(DDR, pin_defs[4].port, pin_defs[4].bit_num, output);
        set_port_bit(PORT, pin_defs[4].port, pin_defs[4].bit_num, val);
        break;

    case 6:
        set_port_bit(DDR, pin_defs[5].port, pin_defs[5].bit_num, output);
        set_port_bit(PORT, pin_defs[5].port, pin_defs[5].bit_num, val);
        break;

    case 7:
        set_port_bit(DDR, pin_defs[6].port, pin_defs[6].bit_num, output);
        set_port_bit(PORT, pin_defs[6].port, pin_defs[6].bit_num, val);
        break;

    case 8:
        set_port_bit(DDR, pin_defs[7].port, pin_defs[7].bit_num, output);
        set_port_bit(PORT, pin_defs[7].port, pin_defs[7].bit_num, val);
        break;

    default: return 0;
    }

    return 1;
}

int set_pin(int pin, int val)
{
    return cfg_pin(pin, 1, val);
}

int get_pin(int pin)
{
    if (pin <= 0 || pin > NUM_GPIO_PINS)
        return 0;

    --pin;
    int bit_num = pin_defs[pin].bit_num;
    if (pin_defs[pin].port == 1) {
        return bit_is_set(PINA, bit_num);
    } else {
        return bit_is_set(PINB, bit_num);
    }
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
