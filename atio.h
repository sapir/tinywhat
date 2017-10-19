#ifndef ATIO_H
#define ATIO_H


#include "io.h"


// a macro to ensure this gets compiled to a single instruction when
// values are constant
#define _set_bit_val(sfr, bit, val) \
    do { \
        if (val) \
            (sfr) |= _BV(bit); \
        else \
            (sfr) &= ~_BV(bit); \
    } while (0)


#endif
