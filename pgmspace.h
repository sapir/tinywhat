#ifndef PGMSPACE_H
#define PGMSPACE_H


#ifdef __AVR__

#include <avr/pgmspace.h>

#else

#define PROGMEM
#define strchr_P strrchr
#define pgm_read_byte(p) (*(p))

#endif


#endif
