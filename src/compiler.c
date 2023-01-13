#include "compiler.h"

#include <stdio.h>

void lua_init_compiler(lua_compiler *compiler) {
    compiler->lexer = NULL;
    compiler->bytecode = NULL;
}

bool lua_compile(lua_compiler *compiler, const char *source, lua_bytecode *bytecode) {
    lua_lexer lexer;
    lua_init_lexer(&lexer, source);

    compiler->lexer = &lexer;
    compiler->bytecode = bytecode;

    return true;
}

static void advance(lua_compiler *compiler) {
    compiler->previous = compiler->current;

    for (;;) {
        compiler->current = lua_next_token(compiler->lexer);
        if (compiler->current.type != TOKEN_ERROR)
            break;

        // TODO: error handling
    }
}

static void consume(lua_compiler *compiler,
                    lua_token_type token_type, const char *message) {
    if (compiler->current.type == token_type) {
        advance(compiler);
        return;
    }

    // TODO: error handling
}


