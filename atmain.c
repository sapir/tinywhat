#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"


int main() {
    uart_setup();

    sei();

    for(;;) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }

    return 0;
}
