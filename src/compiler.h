#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "bytecode.h"

#include <stdbool.h>

typedef struct {
    lua_token previous;
    lua_token current;
} lua_compiler;

bool compile(const char *source, lua_bytecode *bytecode);

#endif
