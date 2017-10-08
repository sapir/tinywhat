#ifndef LEXER_H
#define LEXER_H


#include <stdint.h>
#include <stdbool.h>


// note get_token_size assumes enum order here
enum token_type {
    // no extra data
    TOKEN_NULL,
    TOKEN_LPARENS,
    TOKEN_RPARENS,

    // extra data of sizeof(char)
    TOKEN_KEYWORD,
    TOKEN_VAR,
    TOKEN_UDF,

    // extra data of sizeof(int)
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
    char type;
    union {
        char kwd;
        int8_t var_index;
        signed char udf_name;
        int num;
    };
} __attribute__((packed));

static inline int get_token_type_size(int type) {
    return 1 + (
        type < TOKEN_KEYWORD ? 0
        : type < TOKEN_NUMBER ? sizeof(char)
        : sizeof(int)
    );
}

static inline int get_token_size(struct token *tok) {
    return get_token_type_size(tok->type);
}

static inline struct token *get_next_token(struct token *tok) {
    return (void*)tok + get_token_size(tok);
}


bool lexer_input(char c);

extern struct token last_token;


#endif
