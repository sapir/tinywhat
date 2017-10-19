#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "atio.h"
#include "uart.h"


// receive code based on 'www.technoblogy.com/show?VSX'
// assumes 8MHz clock rate and a calibrated timer

#define UART_BAUD 9600
#define CYCLES_PER_BIT (F_CPU / UART_BAUD)


ISR(PCINT0_vect) {
    // check for pin low
    if (!(PINB & _BV(PINA6))) {
        // disable interrupt
        GIMSK &= ~_BV(PCIE0);

        // set CTC mode, clk_io/8 prescaler, and timer counts
        TCCR0A = 0<<COM0A0 | 0<<COM0B0 | 2<<WGM00;
        TCCR0B = 0<<WGM02 | 2<<CS00;
        OCR0A = 103;
        TCNT0 = 206;

        // enable USI interrupt, 2-wire USI mode, with timer 0 compare match as
        // clock source
        USICR = 1<<USIOIE | 0<<USIWM0 | 1<<USICS0;
        // clear overflow interupt flag, set counter value to 8
        USISR = 1<<USIOIF | 8<<USICNT0;
    }
}

static inline char reverse_byte(char x) {
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;
}

void output(char x) {
}

ISR(USI_OVF_vect) {
    // done with this byte, disable USI
    USICR = 0;
    // save data before it goes away
    int temp = USIDR;
    // pass it on
    output(reverse_byte(temp));

    // re-enable pcint
    GIFR = _BV(PCIF0);
    GIMSK |= _BV(PCIE0);
}

char uart_getc(void)
{
    // TODO
    return 0;
}

void uart_putc(char c)
{
    // start bit
    PORTB &= ~_BV(PORTB2);
    __builtin_avr_delay_cycles(CYCLES_PER_BIT);

    // write bits, lsb first
    for (int i = 0; i < 8; ++i) {
        _set_bit_val(PORTB, PORTB2, c & 1);
        c >>= 1;

        __builtin_avr_delay_cycles(CYCLES_PER_BIT);
    }

    // stop bit
    PORTB &= ~_BV(PORTB2);
    __builtin_avr_delay_cycles(CYCLES_PER_BIT);

    // complete by setting tx to high again
    PORTB |= _BV(PORTB2);
}

void uart_puts(const char *s)
{
    for (;;) {
        char c = *s++;
        if (!c) break;
        uart_putc(c);
    }
}

void uart_putnum(int n)
{
    if (!n) {
        uart_putc('0');
        return;
    }

    if (n < 0) {
        uart_putc('-');
        n = -n;
    }

    while (n) {
        uint8_t digit = n % 10;
        uart_putc(digit + '0');
        n /= 10;
    }
}

void uart_setup(void)
{
    // configure input on rx (PA6)
    DDRA &= ~_BV(DDA6);
    // disable usi
    USICR = 0;
    // clear rx pin interrupt flag
    GIFR = _BV(PCIF0);
    // setup rx pin change interrupt
    GIMSK |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT6);

    // setup tx. set to high before enabling output.
    PORTB |= _BV(PORTB2);
    DDRB |= _BV(DDB2);
}
