#ifndef VALUE_H
#define VALUE_H

#include "common.h"

#include <stdbool.h>
#include <stdlib.h>

typedef double lua_number;

typedef enum {
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOL,
    VALUE_TYPE_NIL,
    VALUE_TYPE_STRING
} value_type;

typedef struct lua_object {
    value_type type;
    bool b;
    lua_number num;
    struct lua_object *next_obj;
    char as[];
} lua_object;

typedef struct {
    size_t len;
    char chars[];
} lua_string;

void lua_print_object(lua_object obj);

lua_object lua_create_nil(void);
lua_object lua_create_bool(bool value);
lua_object lua_create_number(lua_number value);
lua_object lua_create_string(struct lua_vm *vm, char *str, size_t len);

bool lua_is_nil(lua_object obj);
bool lua_is_bool(lua_object obj);
bool lua_is_number(lua_object obj);
bool lua_is_string(lua_object obj);

bool lua_get_bool(lua_object obj);
lua_number lua_get_number(lua_object obj);

bool lua_is_truthy(lua_object obj);
bool lua_is_equal(lua_object a, lua_object b);

#endif
