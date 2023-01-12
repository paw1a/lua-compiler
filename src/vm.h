#ifndef VM_H
#define VM_H

#include "bytecode.h"

#define STACK_MAX_SIZE 256

typedef struct {
    lua_bytecode *bytecode;
    uint8_t *ip;
    lua_object stack[STACK_MAX_SIZE];
    lua_object *sp;
} lua_vm;

void lua_init_vm(lua_vm *vm);
void lua_free_vm(lua_vm *vm);

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} lua_interpret_result;

lua_interpret_result lua_interpret(lua_vm *vm, const char *source);

void lua_push(lua_vm *vm, lua_object value);
lua_object lua_pop(lua_vm *vm);

#endif
