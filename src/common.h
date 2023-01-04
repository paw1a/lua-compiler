#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <assert.h>

// stb based stretchy buffer implementation
struct sb_header {
    uint32_t size;
    uint32_t capacity;
};

#define sb_free(a)         ((a) ? free(stb__sbraw(a)),0 : 0)
#define sb_count(a)        ((a) ? stb__sbn(a) : 0)
#define sb_add(a,n)        (stb__sbmaybegrow(a,n), stb__sbn(a)+=(n), &(a)[stb__sbn(a)-(n)])
#define sb_last(a)         ((a)[stb__sbn(a)-1])

#define sb_header(a) ((struct sb_header *)((char *)(a) - sizeof(struct sb_header)))
#define sb_size(a) ((a) ? sb_header(a)->size : 0)

#define sb_push(a, item) (stb__sbmaybegrow(a, 1), (a)[stb__sbn(a)++] = (item))

#define stb__sbraw(a) ((int *) (a) - 2)
#define stb__sbm(a)   stb__sbraw(a)[0]
#define stb__sbn(a)   stb__sbraw(a)[1]

#define sb_need_grow(a) ((a) == NULL || sb_size(a) + 1 >= sb_header(a)->capacity)
#define sb_maybe_grow(a) (sb_need_grow(a) ? sb_grow(a) : 0)
#define sb_grow(a,n)  stb__sbgrowf((void **) &(a), (n), sizeof(*(a)))

#define min_sb_size 8

static void sb_grow_func(void **sb, size_t item_size)
{
    size_t new_size = *sb ? 2 * sb_header(*sb)->capacity : min_sb_size;
    void *p = realloc(*sb ? sb_header(*sb) : 0, sizeof(struct sb_header) + item_size * new_size);
    assert(p);
    if (p) {
        if (!*arr) ((int *) p)[1] = 0;
        *arr = (void *) ((int *) p + 2);
        stb__sbm(*arr) = m;
    }
}

#endif
