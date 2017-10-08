#ifndef PROGBUF_H
#define PROGBUF_H


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


struct saved_func {
    char name;
    // not using size_t here so test program works on x86_64
    uint16_t size;
    char buf[0];
} __attribute__((packed));


struct saved_func *lookup_func(char name);
// note this "clears" the scratch buffer - 'used' counter is reset but data
// stays put, so as not to invalidate pointers to the scratch buffer, because of
// save_func_from_scratch's flow.
void remove_func(struct saved_func *sf);
void *get_scratch_buf_ptr(void);
size_t get_scratch_buf_used(void);
size_t get_scratch_buf_avail(void);
size_t get_scratch_buf_capacity(void);
// reset 'used' counter
void clear_scratch(void);
// false if no room
bool append_to_scratch(void *buf, size_t size);
// saves buffer as a function and clears scratch buffer, because scratch memory
// is reused to store the function. buf must be at least sizeof(saved_func)
// past the beginning of the scratch buffer. (should be fine because in a 'def'
// statement, a few tokens precede the function contents.)
// returns false if no room.
bool save_func_from_scratch(char name, void *buf, size_t size);


#endif
