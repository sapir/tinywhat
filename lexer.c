#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "pgmspace.h"
#include "vars.h"
#include "eval.h"
#include "lexer.h"



struct token cur_token = { TOKEN_NULL };
struct token last_token = { TOKEN_NULL };
// TOKEN_KEYWORD - number of letters input so far, -1
// TOKEN_NUMBER - number value
int state;


// commit cur_token to last_token, and clear cur_token so we can put a new token
// in it
static void starttoken(void) {
    memcpy(&last_token, &cur_token, sizeof(last_token));

    switch (get_token_type(&last_token)) {
    case TOKEN_NUMBER:
        // use inline token value for short numbers
        // note that state == TOKEN_VAL_MASK is reserved for -1 so we can't
        // use it
        if (0 <= state && state < TOKEN_VAL_MASK) {
            set_token(&last_token, TOKEN_SHORT_NUMBER, state);
        } else {
            set_token_num(&last_token, state);
        }
        break;

    case TOKEN_VAR:
    case TOKEN_UDF:
        if (get_token_val(&last_token) < 0) {
            set_token(&last_token, TOKEN_KEYWORD, KWD_BAD);
        }
        break;

    default: break;
    }

    set_token(&cur_token, TOKEN_NULL, 0);
    state = 0;
}

static bool last_token_is_valid(void) {
    return get_token_type(&last_token) != TOKEN_NULL;
}


struct special_char {
    char c;
    uint8_t type_and_val;
};

static PROGMEM const struct special_char special_chars[] = {
    { '(', _make_token_type_and_val(TOKEN_LPARENS, 0) },
    { ')', _make_token_type_and_val(TOKEN_RPARENS, 0) },
    { '$', _make_token_type_and_val(TOKEN_VAR, -1) },
    { '@', _make_token_type_and_val(TOKEN_UDF, -1) },
    { '=', _make_token_type_and_val(TOKEN_KEYWORD, KWD_eq) },
    { '+', _make_token_type_and_val(TOKEN_KEYWORD, KWD_add) },
    { '-', _make_token_type_and_val(TOKEN_KEYWORD, KWD_sub) },
    { '*', _make_token_type_and_val(TOKEN_KEYWORD, KWD_mul) },
    { '/', _make_token_type_and_val(TOKEN_KEYWORD, KWD_div) },
    { '%', _make_token_type_and_val(TOKEN_KEYWORD, KWD_mod) },

    // whitespace
    { ' ',  _make_token_type_and_val(TOKEN_NULL, 0) },
    { '\t', _make_token_type_and_val(TOKEN_NULL, 0) },
    { '\n', _make_token_type_and_val(TOKEN_NULL, 0) },

    { '\0', 0 },
};

bool lexer_input(char c)
{
    for (int i = 0; pgm_read_byte(&special_chars[i].c); ++i) {
        if (c == pgm_read_byte(&special_chars[i].c)) {
            starttoken();
            cur_token.type_and_val = pgm_read_byte(
                &special_chars[i].type_and_val);
            return last_token_is_valid();
        }
    }

    if (isdigit(c)) {
        bool new = false;
        if (get_token_type(&cur_token) != TOKEN_NUMBER) {
            starttoken();
            set_token(&cur_token, TOKEN_NUMBER, 0);
            new = true;
        }
        state *= 10;
        state += (c - '0');
        return new && last_token_is_valid();
    }

    if (isalpha(c)) {
        c = tolower(c);

        if (get_token_type(&cur_token) == TOKEN_VAR) {
            if (get_token_val(&cur_token) < 0) {
                int index = var_name_to_index(c);
                if (index < 0) {
                    g_eval_error = ERROR_SYNTAX;
                    // invalidate this token
                    set_token(&cur_token, TOKEN_KEYWORD, KWD_BAD);
                } else {
                    set_token_val(&cur_token, index);
                }
            }
            return false;
        }

        if (get_token_type(&cur_token) == TOKEN_UDF) {
            if (get_token_val(&cur_token) < 0) {
                set_token_val(&cur_token, from_udf_name(c));
            }
            return false;
        }

        // not part of a var or udf, must be a keyword

        if (get_token_type(&cur_token) == TOKEN_KEYWORD) {
            // some keywords can't be determined by their first letter. second
            // letter modifies the default.
            if (state == 0) {
                if (get_token_val(&cur_token) == KWD_if && c == 'o') {
                    set_token_val(&cur_token, KWD_io);
                }
            }

            ++state;
            return false;
        }

        starttoken();
        switch (c) {
        case 'c': set_token(&cur_token, TOKEN_KEYWORD, KWD_cfgio); break;
        case 'd': set_token(&cur_token, TOKEN_KEYWORD, KWD_def); break;
        case 'f': set_token(&cur_token, TOKEN_KEYWORD, KWD_for); break;
        case 'i': set_token(&cur_token, TOKEN_KEYWORD, KWD_if); break;
        // case 'i': set_token(&cur_token, TOKEN_KEYWORD, KWD_io); break;
        case 'p': set_token(&cur_token, TOKEN_KEYWORD, KWD_pwm); break;
        case 's': set_token(&cur_token, TOKEN_KEYWORD, KWD_set); break;
        case 'u': set_token(&cur_token, TOKEN_KEYWORD, KWD_undef); break;
        case 'w': set_token(&cur_token, TOKEN_KEYWORD, KWD_wait); break;
        default: set_token(&cur_token, TOKEN_KEYWORD, KWD_BAD); break;
        }
        return last_token_is_valid();
    }

    return false;
}
