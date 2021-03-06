#ifndef LEXER_H
#define LEXER_H


#include <stdint.h>
#include <stdbool.h>


enum token_type {
    TOKEN_NULL,
    TOKEN_LPARENS,
    TOKEN_RPARENS,
    TOKEN_KEYWORD,
    TOKEN_VAR,
    TOKEN_UDF,
    TOKEN_SHORT_NUMBER,

    // has an extra byte
    TOKEN_NUMBER,
};

enum keyword {
    KWD_BAD,
    KWD_add,
    KWD_cfgio,
    KWD_def,
    KWD_div,
    KWD_eq,
    KWD_for,
    KWD_if,
    KWD_io,
    KWD_mod,
    KWD_mul,
    KWD_pwm,
    KWD_set,
    KWD_sub,
    KWD_undef,
    KWD_wait,
};


struct token {
    uint8_t type_and_val;
#define TOKEN_TYPE_MASK 0xe0
#define TOKEN_TYPE_SHIFT 5
#define TOKEN_VAL_MASK 0x1f
#define TOKEN_VAL_SIZE 5

    uint8_t extra_num;
} __attribute__((packed));

static inline int get_token_type_size(int type) {
    return type == TOKEN_NUMBER ? 2 : 1;
}

static inline enum token_type get_token_type(struct token *tok) {
    return tok->type_and_val >> TOKEN_TYPE_SHIFT;
}

static inline int8_t get_token_val(struct token *tok) {
    int8_t val = tok->type_and_val & TOKEN_VAL_MASK;
    // sign extend
    return (val == 0x1f) ? -1 : val;
}

static inline uint8_t make_token_type_and_val(enum token_type type, int8_t val)
{
    if (val < 0) val = 0x1f;
    return (type << TOKEN_TYPE_SHIFT) | val;
}

static inline void set_token(struct token *tok, enum token_type type, int8_t val)
{
    tok->type_and_val = make_token_type_and_val(type, val);
}

static inline void set_token_val(struct token *tok, int8_t val)
{
    if (val < 0) val = 0x1f;
    tok->type_and_val = (tok->type_and_val & ~TOKEN_VAL_MASK) | val;
}

// convert TOKEN_UDF value to a lowercase letter
static inline char to_udf_name(int8_t val)
{
    return (val >= 0) ? (val | 0x60) : -1;
}

static inline char from_udf_name(char c)
{
    return c & TOKEN_VAL_MASK;
}

// for TOKEN_NUMBER
static inline int get_token_num_val(struct token *tok)
{
    int16_t num = tok->extra_num;
    num <<= TOKEN_VAL_SIZE;
    num |= get_token_val(tok);
    return num;
}

static inline void set_token_num(struct token *tok, int num)
{
    // TODO: signed
    set_token(tok, TOKEN_NUMBER, num & TOKEN_VAL_MASK);
    tok->extra_num = num >> TOKEN_VAL_SIZE;
}

static inline int get_token_size(struct token *tok) {
    return get_token_type_size(get_token_type(tok));
}

static inline struct token *get_next_token(struct token *tok) {
    return (void*)tok + get_token_size(tok);
}


bool lexer_input(char c);

extern struct token last_token;


#endif
