#ifndef EVAL_H
#define EVAL_H


#include <stddef.h>
#include "lexer.h"


enum eval_error {
    ERROR_OK,
    ERROR_OOM,
    ERROR_SYNTAX,
    ERROR_DIV_BY_ZERO,
    ERROR_CANT_DEF_IN_UDF,  // can't call (def) from a stored function
};


int eval(struct token *tok, size_t size, size_t *used);


extern enum eval_error g_eval_error;


#endif
