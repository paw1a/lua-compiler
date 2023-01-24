#include "memory.h"

#include <stdlib.h>
#include <string.h>

void *lua_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    void *result = realloc(ptr, new_size);
    return result;
}

void *lua_alloc(size_t size) {
    return lua_realloc(NULL, 0, size);
}

void *lua_calloc(size_t count, size_t size) {
    void *ptr = lua_realloc(NULL, 0, count * size);
    memset(ptr, 0, count * size);
    return ptr;
}

void lua_free(void *ptr) {
    lua_realloc(ptr, 0, 0);
}

void lua_sb_grow(void **arr, size_t item_size) {
    size_t old_size = *arr ? sb_capacity(*arr) : 0;
    size_t new_size = *arr ? 2 * sb_capacity(*arr) : min_sb_size;

    void *ptr = lua_realloc(*arr ? sb_header(*arr) : NULL,
                            sizeof(struct sb_header) + item_size * old_size,
                            sizeof(struct sb_header) + item_size * new_size);
    assert(ptr);

    struct sb_header *header = ptr;
    header->capacity = new_size;
    if (*arr == NULL)
        header->size = 0;

    *arr = header + 1;
}