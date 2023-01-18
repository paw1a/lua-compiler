#include "value.h"
#include "memory.h"
#include "vm.h"

#include <stdio.h>
#include <string.h>

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
    lua_object obj = {VALUE_TYPE_NIL, .num = 0};
    return obj;
}

lua_object lua_create_bool(bool value) {
    lua_object obj = {VALUE_TYPE_BOOL, .b = value};
    return obj;
}

lua_object lua_create_number(lua_number value) {
    lua_object obj = {VALUE_TYPE_NUMBER, .num = value};
    return obj;
}

lua_object lua_create_string(struct lua_vm *vm, char *str, size_t len) {
    void *ptr = lua_alloc(sizeof(lua_object) +
            sizeof(lua_string) + len + 1);
    lua_object *obj = (lua_object *) ptr;
    obj->type = VALUE_TYPE_STRING;

    lua_string *str_obj = (lua_string *) obj->as;
    memcpy(str_obj->chars, str, len);
    str_obj->chars[len] = '\0';
    str_obj->len = len;

    obj->next_obj = vm->obj_list;
    vm->obj_list = obj;

    return *obj;
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

lua_string lua_get_string(lua_object obj) {
    return *(lua_string *) obj.as;
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
    }

    return false;
}
