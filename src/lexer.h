#ifndef LEXER_H
#define LEXER_H

typedef struct {
    char *start;
    char *current;
    int line;
} lua_lexer;

void initialize_lexer(lua_lexer *lexer);

typedef struct {

} lua_token;



#endif
