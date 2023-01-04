#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"

typedef struct {
    lua_token previous;
    lua_token current;
} lua_compiler;

#endif
