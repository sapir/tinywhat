#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"


int main() {
    uart_setup();

    sei();

    uart_puts("tinywhat v1.0\r\n\r\n");

    for(;;) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }

    return 0;
}
