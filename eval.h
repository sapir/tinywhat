#ifndef EVAL_H
#define EVAL_H


#include <stddef.h>
#include "lexer.h"


int eval(struct token *tok, size_t size, size_t *used);


#endif
