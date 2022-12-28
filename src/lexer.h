#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef struct {
    char *start;
    char *current;
    int line;
} lua_lexer;

void initialize_lexer(lua_lexer *lexer, char *source);

typedef enum {
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, // + - * /
    TOKEN_PERCENT, TOKEN_EXP, TOKEN_HASH,             // % ^ #
    TOKEN_EQUAL, TOKEN_NOT_EQUAL,                     // == ~=
    TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL,            // <= >=
    TOKEN_LESS, TOKEN_GREATER,                        // < >
    TOKEN_ASSIGN,                                     // =
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,              // ( )
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,              // { }
    TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,          // [ ]
    TOKEN_COLON, TOKEN_DOUBLE_COLON, TOKEN_SEMICOLON, // : :: ;
    TOKEN_COMMA,                                      // ,
    TOKEN_DOT, TOKEN_DOUBLE_DOT, TOKEN_TRIPLE_DOT,    // . .. ...
    TOKEN_SHIFT_LEFT, TOKEN_SHIFT_RIGHT,              // << >>
    TOKEN_AMPERSAND, TOKEN_PIPE,                      // & |
    TOKEN_DOUBLE_SLASH, TOKEN_TILDE,                  // // ~
    TOKEN_AND, TOKEN_BREAK, TOKEN_DO,                 // and, break, do
    TOKEN_ELSE, TOKEN_ELSEIF, TOKEN_END,              // else, elseif, end
    TOKEN_FALSE, TOKEN_FOR, TOKEN_FUNCTION,           // false, for, function
    TOKEN_GOTO, TOKEN_IF, TOKEN_IN, TOKEN_LOCAL,      // goto, if, in, local
    TOKEN_NIL, TOKEN_NOT, TOKEN_OR,                   // nil, not, or
    TOKEN_REPEAT, TOKEN_RETURN, TOKEN_THEN,           // repeat, return, then
    TOKEN_TRUE, TOKEN_UNTIL, TOKEN_WHILE,             // true, until, while
    TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER,     // number, string, identifier
    TOKEN_EOF, TOKEN_ERROR
} lua_token_type;

typedef struct {
    lua_token_type type;
    const char *start;
    size_t len;
    int line;
} lua_token;

lua_token next_token(lua_lexer *lexer);

#endif
