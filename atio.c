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


static inline void _set_bit_val(uint16_t sfr_addr, int bit, int val)
{
    if (val)
        _SFR_BYTE(sfr_addr) |= _BV(bit);
    else
        _SFR_BYTE(sfr_addr) &= ~_BV(bit);
}

#define set_bit_val(sfr, bit, val) \
    _set_bit_val(_SFR_MEM_ADDR(sfr), bit, val)


int cfg_pin(int pin, int output, int val)
{
    if (pin <= 0 || pin >= NUM_GPIO_PINS)
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
    if (pin <= 0 || pin >= NUM_GPIO_PINS)
        return 0;

    --pin;
    int bit_num = pin_defs[pin].bit_num;
    if (pin_defs[pin].port == 1) {
        set_bit_val(PORTA, bit_num, val);
    } else {
        set_bit_val(PORTB, bit_num, val);
    }

    return val;
}

int get_pin(int pin)
{
    if (pin <= 0 || pin >= NUM_GPIO_PINS)
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
