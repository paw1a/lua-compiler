#include "value.h"
#include "memory.h"
#include "vm.h"

#include <stdio.h>
#include <string.h>

lua_object lua_nil = {.type = VALUE_TYPE_NIL};

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
        case VALUE_TYPE_STRING:
            printf("%s", ((lua_string *) obj.gc_obj)->chars);
            break;
    }
}

lua_object lua_create_nil(void) {
    return lua_nil;
}

lua_object lua_create_bool(bool value) {
    lua_object obj = {VALUE_TYPE_BOOL, .b = value};
    return obj;
}

lua_object lua_create_number(lua_number value) {
    lua_object obj = {VALUE_TYPE_NUMBER, .num = value};
    return obj;
}

lua_object lua_create_gc_obj(lua_gc_object *gc_obj, lua_value_type type) {
    lua_object obj = {type, .gc_obj = gc_obj};
    return obj;
}

lua_object lua_create_string(struct lua_vm *vm, const char *str, size_t len) {
    lua_object obj;
    obj.type = VALUE_TYPE_STRING;

    void *ptr = lua_alloc(sizeof(lua_string) + (len + 1));
    lua_string *str_obj = ptr;

    memcpy(str_obj->chars, str, len);
    str_obj->chars[len] = '\0';
    str_obj->len = len;
    str_obj->hash = lua_hash_string(str_obj);

    obj.gc_obj = (lua_gc_object *) str_obj;

    obj.gc_obj->next = vm->obj_list;
    vm->obj_list = obj.gc_obj;

    return obj;
}

lua_string *alloc_string(size_t len) {
    void *ptr = lua_alloc(sizeof(lua_string) + (len + 1));
    return (lua_string *) ptr;
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

bool lua_is_string(lua_object obj) {
    return obj.type == VALUE_TYPE_STRING;
}

bool lua_get_bool(lua_object obj) {
    return obj.b;
}

lua_number lua_get_number(lua_object obj) {
    return obj.num;
}

lua_string *lua_get_string(lua_object obj) {
    return (lua_string *) obj.gc_obj;
}

// according to lua documentation
// false and nil are falsy values
bool lua_is_truthy(lua_object obj) {
    if (lua_is_nil(obj))
        return false;
    else if (lua_is_bool(obj) && !lua_get_bool(obj))
        return false;
    return true;
}

bool lua_is_equal(lua_object a, lua_object b) {
    if (a.type != b.type)
        return false;

    switch (a.type) {
        case VALUE_TYPE_NUMBER:
            return lua_get_number(a) == lua_get_number(b);
        case VALUE_TYPE_BOOL:
            return lua_get_bool(a) == lua_get_bool(b);
        case VALUE_TYPE_NIL:
            return true;
        case VALUE_TYPE_STRING: {
            lua_string *str_a = lua_get_string(a);
            lua_string *str_b = lua_get_string(b);
            return str_a->len == str_b->len &&
                memcmp(str_a->chars, str_b->chars, str_a->len) == 0;
        }
    }

    return false;
}

uint32_t lua_hash_string(lua_string *str) {
    uint32_t hash = 2166136261u;

    for (int i = 0; i < str->len; i++) {
        hash ^= (uint8_t) str->chars[i];
        hash *= 16777619;
    }

    return hash;
}

uint32_t lua_hash_number(lua_number number) {
    uint32_t hash = 2166136261u;

    char *ptr = (char *) &number;
    for (int i = 0; i < sizeof(lua_number); i++) {
        hash ^= (uint8_t) ptr[i];
        hash *= 16777619;
    }

    return hash;
}

uint32_t lua_hash_object(lua_object obj) {
    switch (obj.type) {
        case VALUE_TYPE_NUMBER:
            return lua_hash_number(obj.num);
        case VALUE_TYPE_STRING:
            return ((lua_string *) obj.gc_obj)->hash;
        case VALUE_TYPE_BOOL:
            return obj.b;
        default:
            return 0;
    }
}
