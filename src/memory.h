#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <assert.h>

// stb based stretchy buffer implementation
struct sb_header {
    size_t size;
    size_t capacity;
};

#define sb_header(a) ((struct sb_header *)((char *)(a) - sizeof(struct sb_header)))
#define sb_size(a) ((a) ? sb_header(a)->size : 0)
#define sb_capacity(a) ((a) ? sb_header(a)->capacity : 0)

#define sb_push(a, item) (sb_maybe_grow(a), (a)[sb_header(a)->size++] = (item))
#define sb_free(a) ((a) ? free(sb_header(a)),0 : 0)
#define sb_last(a) ((a)[sb_size(a) - 1])

#define sb_need_grow(a) ((a) == NULL || sb_size(a) + 1 >= sb_capacity(a))
#define sb_maybe_grow(a) (sb_need_grow(a) ? sb_grow(a) : 0)
#define sb_grow(a) lua_sb_grow((void **) &(a), sizeof(*(a)))

#define min_sb_size 8

void lua_sb_grow(void **sb, size_t item_size);
void *lua_realloc(void *pointer, size_t old_size, size_t new_size);
void *lua_alloc(size_t size);
void *lua_calloc(size_t count, size_t size);
void lua_free(void *ptr);

#endif
