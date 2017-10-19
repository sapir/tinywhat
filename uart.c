#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"


// receive code based on 'www.technoblogy.com/show?RPY'
// assumes 8MHz clock rate and a calibrated timer


ISR(PCINT0_vect) {
    if (!(PINB & _BV(PINB0))) {
        GIMSK &= ~_BV(PCIE);
        TCCR0A = 2<<WGM00;
        TCCR0B = 0<<WGM02 | 2<<CS00;
        TCNT0 = 0;
        OCR0A = 51;
        TIFR |= _BV(OCF0A);
        TIMSK |= _BV(OCIE0A);
    }
}

ISR(TIMER0_COMPA_vect) {
    TIMSK &= ~_BV(OCIE0A);
    TCNT0 = 0;
    OCR0A = 103;
    USICR = 1<<USIOIE | 0<<USIWM0 | 1<<USICS0;
    USISR = 1<<USIOIF | 8;
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
    USICR = 0;
    int temp = USIDR;
    output(reverse_byte(temp));
    GIFR = _BV(PCIF);
    GIMSK |= _BV(PCIE);
}

char uart_getc(void)
{
    // TODO
    return 0;
}

void uart_putc(char c)
{
    // TODO
}

void uart_setup(void)
{
    DDRB &= ~_BV(0);
    USICR = 0;
    GIFR = _BV(PCIF);
    GIMSK |= _BV(PCIE);
    PCMSK |= _BV(PCINT0);
}
