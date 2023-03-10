#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "bytecode.h"
#include "vm.h"

#include <stdbool.h>

typedef struct {
    lua_token previous;
    lua_token current;

    lua_lexer *lexer;
    lua_bytecode *bytecode;

    lua_vm *vm;
} lua_compiler;

void lua_init_compiler(lua_compiler *compiler, lua_vm *vm);
bool lua_compile(lua_compiler *compiler, const char *source, lua_bytecode *bytecode);

#endif
