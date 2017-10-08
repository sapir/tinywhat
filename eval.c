#include <stdio.h>
#include "lexer.h"
#include "vars.h"
#include "progbuf.h"
#include "eval.h"


static struct token *get_buf_end(struct token *tok, size_t size)
{
    return (struct token*)((void*)tok + size);
}

// find size of function call, starting at lparens
static size_t find_rparens(struct token *tok, size_t size)
{
    char num_parens = 0;
    struct token *start = tok;
    struct token *end = get_buf_end(tok, size);
    struct token *next = get_next_token(tok);
    while (next <= end) {
        if (tok->type == TOKEN_LPARENS) ++num_parens;
        else if (tok->type == TOKEN_RPARENS) --num_parens;

        if (num_parens == 0) return (char*)tok - (char*)start;

        tok = next;
        next = get_next_token(tok);
    }

    return 0;
}

static size_t get_arg_size(struct token *tok, size_t size)
{
    switch (tok->type) {
    case TOKEN_VAR:
    case TOKEN_UDF:
    case TOKEN_NUMBER:
        return get_token_size(tok);

    case TOKEN_LPARENS:
        return find_rparens(tok, size);

    default:
        return 0;
    }
}

#define more_args(tok, size) (size > 0 && tok->type != TOKEN_RPARENS)

#define skip_arg(tok, size, used) \
    do { \
        size -= (used); \
        tok = (void*)tok + (used); \
    } while(0)

#define set_next_arg(val, tok, size) \
    do { \
        size_t __used; \
        val = eval(tok, size, &__used); \
        if (!__used) { printf("syntax error\n"); return 0; } \
        skip_arg(tok, size, __used); \
    } while(0)

#define foreach_args(val, tok, size) \
    while (more_args(tok, size)) { \
        set_next_arg(val, tok, size); \

#define end_foreach_args() \
    }


// evaluate expressions until out of room, return last value
static int exec_block(struct token *tok, size_t size)
{
    int val;
    foreach_args(val, tok, size) { (void)val; } end_foreach_args();
    return val;
}


static int func_add(struct token *tok, size_t size)
{
    int res = 0;
    int val;
    foreach_args(val, tok, size) {
        res += val;
    } end_foreach_args();

    return res;
}

static int func_cfgio(struct token *tok, size_t size)
{
    printf("cfgio\n");
    return 0;
}

static int func_def(struct token *tok, size_t size)
{
    if (tok->type != TOKEN_UDF) {
        printf("syntax error\n");
        return 0;
    }

    char name = tok->udf_name;
    skip_arg(tok, size, get_token_type_size(TOKEN_UDF));

    // strip off useless rparens, don't bother saving it
    size -= get_token_type_size(TOKEN_RPARENS);

    if (!is_in_scratch(tok)) {
        printf("can't call (def) from a stored function\n");
        return 0;
    }

    if (!save_func_from_scratch(name, tok, size)) {
        printf("oom\n");
        return 0;
    }

    return 1;
}

static int func_div(struct token *tok, size_t size)
{
    if (!more_args(tok, size)) return 0;

    int res;
    set_next_arg(res, tok, size);

    int val;
    foreach_args(val, tok, size) {
        if (!val) {
            printf("division by zero\n");
            return 0;
        }

        res /= val;
    } end_foreach_args();

    return res;
}

static int func_eq(struct token *tok, size_t size)
{
    if (!more_args(tok, size)) return 0;

    int val1;
    set_next_arg(val1, tok, size);

    int val2;
    foreach_args(val2, tok, size) {
        if (val2 != val1) return 0;
    } end_foreach_args();

    return 1;
}

static int func_for(struct token *tok, size_t size)
{
    if (tok->type != TOKEN_VAR) {
        printf("syntax error\n");
        return 0;
    }

    int var = tok->var_index;
    skip_arg(tok, size, get_token_type_size(TOKEN_VAR));

    int start;
    if (!more_args(tok, size)) {
        printf("syntax error\n");
        return 0;
    }
    set_next_arg(start, tok, size);

    int end;
    if (!more_args(tok, size)) {
        printf("syntax error\n");
        return 0;
    }
    set_next_arg(end, tok, size);

    int val = 0;
    for (int i = start; i < end; ++i) {
        vars[var] = i;
        val = exec_block(tok, size);
    }

    return val;
}

static int func_if(struct token *tok, size_t size)
{
    if (!more_args(tok, size)) return 0;

    int val;
    set_next_arg(val, tok, size);

    if (!val) {
        if (!more_args(tok, size)) return 0;
        size_t used = get_arg_size(tok, size);
        skip_arg(tok, size, used);
    }

    if (!more_args(tok, size)) return 0;
    set_next_arg(val, tok, size);
    return val;
}

static int func_io(struct token *tok, size_t size)
{
    printf("io\n");
    return 0;
}

static int func_set(struct token *tok, size_t size)
{
    if (tok->type != TOKEN_VAR) {
        printf("syntax error\n");
        return 0;
    }

    int var = tok->var_index;
    size -= get_token_size(tok);
    tok = get_next_token(tok);
    if (!more_args(tok, size)) {
        printf("syntax error\n");
        return 0;
    }

    int val;
    set_next_arg(val, tok, size);
    vars[var] = val;
    return val;
}

static int func_mod(struct token *tok, size_t size)
{
    if (!more_args(tok, size)) return 0;

    int res;
    set_next_arg(res, tok, size);

    int val;
    foreach_args(val, tok, size) {
        if (!val) {
            printf("division by zero\n");
            return 0;
        }

        res %= val;
    } end_foreach_args();

    return 0;
}

static int func_mul(struct token *tok, size_t size)
{
    int res = 1;
    int val;
    foreach_args(val, tok, size) {
        res *= val;
    } end_foreach_args();

    return res;
}

static int func_pwm(struct token *tok, size_t size)
{
    printf("pwm\n");
    return 0;
}

static int func_sub(struct token *tok, size_t size)
{
    if (!more_args(tok, size)) return 0;

    int res;
    set_next_arg(res, tok, size);

    int val;
    foreach_args(val, tok, size) {
        res -= val;
    } end_foreach_args();

    return res;
}

static int func_wait(struct token *tok, size_t size)
{
    printf("wait\n");
    return 0;
}

static int eval_func(struct token *tok, size_t size)
{
    // identify function
    tok = get_next_token(tok);
    // tok must be a valid token - first token must have been an lparens to
    // reach here, and parens must cancel out, so there must be at least 2
    // tokens

    switch (tok->type) {
    case TOKEN_KEYWORD:
        {
            char kwd = tok->kwd;
            size -= get_token_size(tok);
            tok = get_next_token(tok);
            switch (kwd) {
            case KWD_add:   return func_add(tok, size);
            case KWD_cfgio: return func_cfgio(tok, size);
            case KWD_def:   return func_def(tok, size);
            case KWD_div:   return func_div(tok, size);
            case KWD_eq:    return func_eq(tok, size);
            case KWD_for:   return func_for(tok, size);
            case KWD_if:    return func_if(tok, size);
            case KWD_io:    return func_io(tok, size);
            case KWD_mod:   return func_mod(tok, size);
            case KWD_mul:   return func_mul(tok, size);
            case KWD_pwm:   return func_pwm(tok, size);
            case KWD_set:   return func_set(tok, size);
            case KWD_sub:   return func_sub(tok, size);
            case KWD_wait:  return func_wait(tok, size);
            }
            break;
        }

    case TOKEN_UDF:
        {
            struct saved_func *sf = lookup_func(tok->udf_name);
            if (!sf) {
                return 0;
            }

            // TODO: arguments?
            return exec_block((struct token*)sf->buf, sf->size);
        }
    }

    printf("syntax error\n");
    return 0;
}

int eval(struct token *tok, size_t size, size_t *used)
{
    switch (tok->type) {
    case TOKEN_VAR:
        *used = get_token_size(tok);
        return vars[tok->var_index];

    case TOKEN_NUMBER:
        *used = get_token_size(tok);
        return tok->num;

    case TOKEN_LPARENS:
        {
            size_t rparens = find_rparens(tok, size);
            if (rparens > 0) {
                *used = rparens;
                return eval_func(tok, rparens);
            }
            break;
        }
    }

    printf("syntax error\n");
    *used = 0;
    return 0;
}
