#include "compiler.h"

#include <stdio.h>

bool compile(const char *source, lua_bytecode *bytecode) {
    lua_lexer lexer;
    lua_init_lexer(&lexer, source);

    int line = -1;
    for (;;) {
        lua_token token = lua_next_token(&lexer);
        if (token.line != line) {
            printf("%4d ", token.line + 1);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, (int)token.len, token.start);

        if (token.type == TOKEN_EOF)
            break;
    }
}
