#include <stdio.h>
#include "lexer.h"


int main()
{
    for (;;) {
        char c = getc(stdin);
        if (c == EOF)
            break;

        if (lexer_input(c)) {
            printf("token: ");
            switch (last_token.type) {
            case TOKEN_LPARENS: printf("(\n"); break;
            case TOKEN_RPARENS: printf(")\n"); break;
            case TOKEN_KEYWORD:
                switch (last_token.kwd) {
                case KWD_BAD: printf("BAD\n"); break;
                case KWD_add: printf("add\n"); break;
                case KWD_cfgio: printf("cfgio\n"); break;
                case KWD_def: printf("def\n"); break;
                case KWD_div: printf("div\n"); break;
                case KWD_eq: printf("eq\n"); break;
                case KWD_for: printf("for\n"); break;
                case KWD_if: printf("if\n"); break;
                case KWD_io: printf("io\n"); break;
                case KWD_let: printf("let\n"); break;
                case KWD_mod: printf("mod\n"); break;
                case KWD_mul: printf("mul\n"); break;
                case KWD_pwm: printf("pwm\n"); break;
                case KWD_sub: printf("sub\n"); break;
                case KWD_wait: printf("wait\n"); break;
                }
                break;

            case TOKEN_NUMBER: printf("%d\n", last_token.num); break;
            case TOKEN_VAR: printf("%c\n", last_token.var + 'A'); break;
            }
        }
    }

    return 0;
}
