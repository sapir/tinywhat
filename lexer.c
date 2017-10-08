#include <ctype.h>
#include <string.h>
#include "lexer.h"



struct token cur_token = { TOKEN_NULL };
struct token last_token = { TOKEN_NULL };
int state;


static void starttoken(void) {
    memcpy(&last_token, &cur_token, sizeof(last_token));
    memset(&cur_token, 0, sizeof(cur_token));
    state = 0;
}

static bool last_token_is_valid(void) {
    return last_token.type != TOKEN_NULL;
}

bool lexer_input(char c)
{
    if (isspace(c) || c == '(' || c == ')' || c == '$'
        || c == '+' || c == '/' || c == '=' || c == '%' || c == '*'
        || c == '-' || c == '@')
    {
        starttoken();
        switch (c) {
        case '(': cur_token.type = TOKEN_LPARENS; break;
        case ')': cur_token.type = TOKEN_RPARENS; break;
        case '$': cur_token.type = TOKEN_VAR; break;
        case '@': cur_token.type = TOKEN_UDF; break;
        case '=': cur_token.type = TOKEN_KEYWORD; cur_token.kwd = KWD_eq; break;
        case '+': cur_token.type = TOKEN_KEYWORD; cur_token.kwd = KWD_add; break;
        case '-': cur_token.type = TOKEN_KEYWORD; cur_token.kwd = KWD_sub; break;
        case '*': cur_token.type = TOKEN_KEYWORD; cur_token.kwd = KWD_mul; break;
        case '/': cur_token.type = TOKEN_KEYWORD; cur_token.kwd = KWD_div; break;
        case '%': cur_token.type = TOKEN_KEYWORD; cur_token.kwd = KWD_mod; break;
        default: cur_token.type = TOKEN_NULL; break;
        }
        return last_token_is_valid();
    }

    if (isdigit(c)) {
        bool new = false;
        if (cur_token.type != TOKEN_NUMBER) {
            starttoken();
            cur_token.type = TOKEN_NUMBER;
            new = true;
        }
        cur_token.num *= 10;
        cur_token.num += (c - '0');
        return new && last_token_is_valid();
    }

    if (isalpha(c)) {
        c = tolower(c);

        if (cur_token.type == TOKEN_VAR || cur_token.type == TOKEN_UDF) {
            if (cur_token.var == 0)
                cur_token.var = c - 'a' + 1;
            return false;
        }

        // not part of a var, must be a keyword

        if (cur_token.type == TOKEN_KEYWORD) {
            // some keywords can't be determined by their first letter. second
            // letter modifies the default.
            if (state == 0) {
                if (cur_token.kwd == KWD_if && c == 'o') {
                    cur_token.kwd = KWD_io;
                }
            }

            ++state;
            return false;
        }

        starttoken();
        cur_token.type = TOKEN_KEYWORD;
        switch (c) {
        case 'c': cur_token.kwd = KWD_cfgio; break;
        case 'd': cur_token.kwd = KWD_def; break;
        case 'f': cur_token.kwd = KWD_for; break;
        case 'i': cur_token.kwd = KWD_if; break;
        // case 'i': cur_token.kwd = KWD_io; break;
        case 'p': cur_token.kwd = KWD_pwm; break;
        case 's': cur_token.kwd = KWD_set; break;
        case 'w': cur_token.kwd = KWD_wait; break;
        default: cur_token.kwd = KWD_BAD; break;
        }
        return last_token_is_valid();
    }

    return false;
}
