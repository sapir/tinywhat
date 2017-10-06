#include <avr/interrupt.h>
#include <avr/sleep.h>


int main() {
    sei();

    for(;;) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }

    return 0;
}
