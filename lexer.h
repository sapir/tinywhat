#ifndef LEXER_H
#define LEXER_H


#include <stdbool.h>


enum token_type {
    TOKEN_NULL,
    TOKEN_LPARENS,
    TOKEN_RPARENS,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_VAR,
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
    KWD_let,
    KWD_mod,
    KWD_mul,
    KWD_pwm,
    KWD_sub,
    KWD_wait,
};


struct token {
    char type;
    union {
        char kwd;
        char var;
        int num;
    };
} __attribute__((packed));

static inline int get_token_size(struct token *tok) {
    return 1 + (tok->type == TOKEN_NUMBER ? sizeof(int) : sizeof(char));
}

static inline struct token *get_next_token(struct token *tok) {
    return (void*)tok + get_token_size(tok);
}


bool lexer_input(char c);

extern struct token last_token;


#endif
