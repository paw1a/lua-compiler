#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef double lua_number;

typedef enum {
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOL,
    VALUE_TYPE_NIL
} value_type;

typedef struct {
    value_type type;
    union {
        bool b;
        lua_number num;
    } as;
} lua_object;

void lua_print_object(lua_object obj);

lua_object lua_create_nil(void);
lua_object lua_create_bool(bool value);
lua_object lua_create_number(lua_number value);

bool lua_is_nil(lua_object obj);
bool lua_is_bool(lua_object obj);
bool lua_is_number(lua_object obj);

bool lua_get_bool(lua_object obj);
lua_number lua_get_number(lua_object obj);

#endif
