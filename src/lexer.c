#include "lexer.h"

#include <string.h>

void initialize_lexer(lua_lexer *lexer, char *source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 0;
}

static lua_token create_token(lua_lexer *lexer, lua_token_type type) {
    lua_token token;

    token.type = type;
    token.start = lexer->start;
    token.len = lexer->current - lexer->current;
    token.line = lexer->line;

    return token;
}

static lua_token create_error_token(lua_lexer *lexer, const char *message) {
    lua_token token;

    token.type = TOKEN_ERROR;
    token.start = message;
    token.len = strlen(message);
    token.line = lexer->line;

    return token;
}

lua_token next_token(lua_lexer *lexer) {
    lexer->start = lexer->current;

    if (*lexer->current == '\0') {
        return create_token(lexer, TOKEN_EOF);
    }


}
