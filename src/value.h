#ifndef VALUE_H
#define VALUE_H

#include "common.h"

#include <stdbool.h>
#include <stdlib.h>

typedef double lua_number;

typedef enum {
    VALUE_TYPE_NIL,
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOL,
    VALUE_TYPE_STRING
} lua_value_type;

typedef struct lua_object lua_object;
typedef struct lua_gc_object lua_gc_object;

struct lua_gc_object {
    lua_gc_object *next;
};

struct lua_object {
    lua_value_type type;
    bool b;
    lua_number num;
    lua_gc_object *gc_obj;
};

typedef struct {
    lua_gc_object gc_obj;
    size_t len;
    uint32_t hash;
    char chars[];
} lua_string;

extern lua_object lua_nil;

void lua_print_object(lua_object obj);

lua_object lua_create_nil(void);
lua_object lua_create_bool(bool value);
lua_object lua_create_number(lua_number value);
lua_object lua_create_gc_obj(lua_gc_object *gc_obj, lua_value_type type);
lua_object lua_create_string(struct lua_vm *vm, const char *str, size_t len);

lua_string *alloc_string(size_t len);

static inline bool lua_is_nil(lua_object obj) {
    return obj.type == VALUE_TYPE_NIL;
}

static inline bool lua_is_bool(lua_object obj) {
    return obj.type == VALUE_TYPE_BOOL;
}

static inline bool lua_is_number(lua_object obj) {
    return obj.type == VALUE_TYPE_NUMBER;
}

static inline bool lua_is_integer(lua_object obj) {
    return obj.type == VALUE_TYPE_NUMBER &&
           (int32_t) obj.num == obj.num;
}

static inline bool lua_is_string(lua_object obj) {
    return obj.type == VALUE_TYPE_STRING;
}

static inline bool lua_get_bool(lua_object obj) {
    return obj.b;
}

static inline lua_number lua_get_number(lua_object obj) {
    return obj.num;
}

static inline lua_string *lua_get_string(lua_object obj) {
    return (lua_string *) obj.gc_obj;
}

bool lua_is_truthy(lua_object obj);
bool lua_is_equal(lua_object a, lua_object b);

uint32_t lua_hash_object(lua_object obj);
uint32_t lua_hash_string(lua_string *str);
uint32_t lua_hash_number(lua_number number);

#endif
