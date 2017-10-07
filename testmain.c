#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "eval.h"


char exec_buf[256];
size_t exec_buf_used = 0;
char num_parens = 0;


static void execute(char *buf, size_t size)
{
    struct token *tok = (struct token*)buf;
    assert((char*)get_next_token(tok) <= buf + size);
    size_t used;
    int res = eval(tok, size, &used);
    printf(" -> %d\n", res);
}

int main()
{
    for (;;) {
        char c = getc(stdin);
        if (c == EOF)
            break;

        if (lexer_input(c)) {
            int tok_size = get_token_size(&last_token);
            if (tok_size > sizeof(exec_buf) - exec_buf_used) {
                printf("oom, clearing exec buf\n");
                exec_buf_used = 0;
                num_parens = 0;
            } else {
                memcpy(exec_buf + exec_buf_used, &last_token, tok_size);
                exec_buf_used += tok_size;
            }

            if (last_token.type == TOKEN_LPARENS) ++num_parens;
            else if (last_token.type == TOKEN_RPARENS) --num_parens;

            if (num_parens == 0) {
                execute(exec_buf, exec_buf_used);
                exec_buf_used = 0;
            }
        }
    }

    return 0;
}
