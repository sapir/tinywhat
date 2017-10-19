#ifndef UART_H
#define UART_H


void uart_setup(void);


char uart_getc(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_putnum(int n);


#endif
