#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "eval.h"
#include "progbuf.h"


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
    int num_parens = 0;

    for (;;) {
        char c = getc(stdin);
        if (c == EOF)
            break;

        if (lexer_input(c)) {
            int tok_size = get_token_size(&last_token);
            if (!append_to_scratch(&last_token, tok_size)) {
                printf("oom, clearing scratch buf\n");
                clear_scratch();
                num_parens = 0;
                continue;
            }

            if (get_token_type(&last_token) == TOKEN_LPARENS) ++num_parens;
            else if (get_token_type(&last_token) == TOKEN_RPARENS) --num_parens;

            if (num_parens == 0) {
                execute(get_scratch_buf_ptr(), get_scratch_buf_used());
                clear_scratch();
            }
        }
    }

    return 0;
}
