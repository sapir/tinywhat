#include <assert.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"
#include "lexer.h"
#include "progbuf.h"
#include "eval.h"


static void execute(char *buf, size_t size)
{
    struct token *tok = (struct token*)buf;
    assert((char*)get_next_token(tok) <= buf + size);
    size_t used;
    int res = eval(tok, size, &used);
    // TODO: display errors
    uart_puts(" -> ");
    uart_putnum(res);
    uart_puts("\r\n");
}

void __attribute__((noreturn)) repl_loop(void)
{
    int num_parens = 0;

    for(;;) {
        char c = uart_getc();
        if (c) {
            uart_putc(c);

            if (lexer_input(c)) {
                int tok_size = get_token_size(&last_token);
                if (!append_to_scratch(&last_token, tok_size)) {
                    uart_puts("oom, clearing scratch buf\r\n");
                    clear_scratch();
                    num_parens = 0;
                    continue;
                }

                if (get_token_type(&last_token) == TOKEN_LPARENS) {
                    ++num_parens;
                } else if (get_token_type(&last_token) == TOKEN_RPARENS) {
                    --num_parens;

                    if (num_parens == 0) {
                        execute(get_scratch_buf_ptr(), get_scratch_buf_used());
                        clear_scratch();
                    }
                }
            }
        }

        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
}


int main() {
    uart_setup();

    sei();

    uart_puts("tinywhat v1.0\r\n\r\n");

    repl_loop();
}
