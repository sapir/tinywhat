#ifndef VARS_H
#define VARS_H


#define NUM_VARS 8


extern int vars[NUM_VARS];


// returns -1 if invalid
int var_name_to_index(char name);
char var_index_to_name(int index);


#endif
