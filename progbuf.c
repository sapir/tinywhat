#include <assert.h>
#include <string.h>
#include "progbuf.h"


// must be enough for lparens, unset/undef, name token, and rparens
#define MIN_SCRATCH_CAPACITY 6


static char global_buf[128];
static size_t used_for_funcs = 0;
static size_t used_for_scratch = 0;


#define MAX_FUNCS_SIZE (sizeof(global_buf) - MIN_SCRATCH_CAPACITY)


static size_t get_saved_func_size(struct saved_func *sf)
{
    return sizeof(struct saved_func) + sf->size;
}


struct saved_func *lookup_func(char name)
{
    size_t i = 0;
    while (i < used_for_funcs) {
        struct saved_func *sf = (struct saved_func*)&global_buf[i];
        if (sf->name == name)
            return sf;

        i += get_saved_func_size(sf);
    }

    return NULL;
}

void remove_func(struct saved_func *sf)
{
    size_t sf_size = get_saved_func_size(sf);
    void *sf_end = (void*)sf + sf_size;
    // only move bit up to used_for_funcs, so we don't ruin existing pointers
    // to scratch buffer when this is called from save_func_from_scratch().
    memmove(sf, sf_end, ((void*)global_buf + used_for_funcs) - sf_end);
    used_for_funcs -= sf_size;
    clear_scratch();
}

void *get_scratch_buf_ptr(void)
{
    return global_buf + used_for_funcs;
}

size_t get_scratch_buf_used(void)
{
    return used_for_scratch;
}

size_t get_scratch_buf_capacity(void)
{
    return sizeof(global_buf) - used_for_funcs;
}

size_t get_scratch_buf_avail(void)
{
    return get_scratch_buf_capacity() - get_scratch_buf_used();
}

void clear_scratch(void)
{
    used_for_scratch = 0;
}

bool append_to_scratch(void *buf, size_t size)
{
    if (get_scratch_buf_avail() < size)
        return false;

    char *scratch = get_scratch_buf_ptr();
    memcpy(scratch + used_for_scratch, buf, size);
    used_for_scratch += size;
    return true;
}

bool save_func_from_scratch(char name, void *buf, size_t size)
{
    assert(buf - get_scratch_buf_ptr() >= sizeof(struct saved_func));

    struct saved_func *old = lookup_func(name);

    // make sure we'll have enough room after saving
    size_t funcs_size_after_save
        = used_for_funcs + sizeof(struct saved_func) + size;
    if (old) {
        funcs_size_after_save -= (sizeof(struct saved_func) + old->size);
    }
    if (funcs_size_after_save > MAX_FUNCS_SIZE)
        return false;

    // we have enough room, now we can commit - remove the old definition.
    if (old)
        remove_func(old);

    struct saved_func *sf = (struct saved_func*)(global_buf + used_for_funcs);
    used_for_funcs = funcs_size_after_save;
    sf->name = name;
    sf->size = size;
    memmove(sf->buf, buf, size);

    clear_scratch();

    return true;
}
