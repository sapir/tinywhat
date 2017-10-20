#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "atio.h"
#include "uart.h"


// receive code based on 'www.technoblogy.com/show?VSX'
// assumes 8MHz clock rate and a calibrated timer

#define UART_BAUD 9600
#define CYCLES_PER_BIT (F_CPU / UART_BAUD)


char rx_buf;


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

    rx_buf = reverse_byte(temp);

    // re-enable pcint
    GIFR = _BV(PCIF0);
    GIMSK |= _BV(PCIE0);
}

char uart_getc(void)
{
    char c = rx_buf;
    rx_buf = '\0';
    return c;
}

void uart_putc(char c)
{
    // set up value to be transmitted, including zero start & stop bits, and a 1
    // bit after the stop bit so that output is left high. note that value will
    // be transmitted lsb first, so start bit is lsb.
    int val = 0 | (c << 1) | 0 << 9 | 1 << 10;

    for (int i = 0; i < 11; ++i) {
        _set_bit_val(PORTB, PORTB2, val & 1);
        val >>= 1;

        // this is unnecessary on the last iteration, but it's easier to keep it
        __builtin_avr_delay_cycles(CYCLES_PER_BIT);
    }
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

    char buf[6];
    int i = 0;
    while (n) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    do {
        uart_putc(buf[--i]);
    } while (i);
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
