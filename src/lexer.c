#include "lexer.h"

void initialize_lexer(lua_lexer *lexer, char *source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 0;
}
