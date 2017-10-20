#include <string.h>
#include "pgmspace.h"
#include "vars.h"


static PROGMEM const char var_names[] = "abcdeijk";


int vars[NUM_VARS];


int var_name_to_index(char name)
{
    for (int i = 0; pgm_read_byte(&var_names[i]); ++i) {
        if (pgm_read_byte(&var_names[i]) == name) {
            return i;
        }
    }

    return -1;
}

char var_index_to_name(int index)
{
    return pgm_read_byte(&var_names[index]);
}
