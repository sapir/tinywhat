#include <string.h>
#include "vars.h"


const char var_names[] = "abcdeijk";


int vars[NUM_VARS];


int var_name_to_index(char name)
{
    char *p = strchr(var_names, name);
    return p ? p - var_names : -1;
}

char var_index_to_name(int index)
{
    return var_names[index];
}
