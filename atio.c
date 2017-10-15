#include <stdbool.h>
#include <avr/io.h>
#include "io.h"


struct pin_def {
    int port;
    int bit_num;
};

#define NUM_GPIO_PINS 8

static const struct pin_def pin_defs[] = {
    { 2, 0 },
    { 2, 1 },
    { 1, 7 },
    { 1, 6 },
    { 1, 5 },
    { 1, 4 },
    { 1, 3 },
    { 1, 2 },
};


// a macro to ensure this gets compiled to a single instruction when
// values are constant
#define _set_bit_val(sfr, bit, val) \
    do { \
        if (val) \
            (sfr) |= _BV(bit); \
        else \
            (sfr) &= ~_BV(bit); \
    } while (0)


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


int cfg_pin(int pin, int output, int val)
{
    if (pin <= 0 || pin > NUM_GPIO_PINS)
        return 0;

    --pin;
    int bit_num = pin_defs[pin].bit_num;
    if (pin_defs[pin].port == 1) {
        set_bit_val(DDRA, bit_num, output);
        set_bit_val(PORTA, bit_num, val);
    } else {
        set_bit_val(DDRB, bit_num, output);
        set_bit_val(PORTB, bit_num, val);
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
