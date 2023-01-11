#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef double lua_number;

typedef enum {
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOL,
    VALUE_TYPE_STRING,
} value_type;

typedef union {
    bool b;
    lua_number num;
    void *p;
} lua_value;

typedef struct {
    value_type type;
    lua_value value;
} lua_object;

void lua_print_object(lua_object obj);

#endif
