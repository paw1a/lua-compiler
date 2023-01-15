#include "value.h"

#include <stdio.h>

void lua_print_object(lua_object obj) {
    switch (obj.type) {
        case VALUE_TYPE_NUMBER:
            printf("%g", lua_get_number(obj));
            break;
        case VALUE_TYPE_BOOL:
            printf(lua_get_bool(obj) ? "true" : "false");
            break;
        case VALUE_TYPE_NIL:
            printf("nil");
            break;
    }
}

lua_object lua_create_nil(void) {
    lua_object obj = {VALUE_TYPE_NIL, {.num = 0}};
    return obj;
}

lua_object lua_create_bool(bool value) {
    lua_object obj = {VALUE_TYPE_BOOL, {.b = value}};
    return obj;
}

lua_object lua_create_number(lua_number value) {
    lua_object obj = {VALUE_TYPE_NUMBER, {.num = value}};
    return obj;
}


bool lua_is_nil(lua_object obj) {
    return obj.type == VALUE_TYPE_NIL;
}

bool lua_is_bool(lua_object obj) {
    return obj.type == VALUE_TYPE_BOOL;
}

bool lua_is_number(lua_object obj) {
    return obj.type == VALUE_TYPE_NUMBER;
}


bool lua_get_bool(lua_object obj) {
    return obj.as.b;
}

lua_number lua_get_number(lua_object obj) {
    return obj.as.num;
}

