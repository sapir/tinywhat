#ifndef EVAL_H
#define EVAL_H


#include "lexer.h"


int eval(struct token *tok, size_t size, size_t *used);


#endif
