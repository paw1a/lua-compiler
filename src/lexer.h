#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    LEXER_STATE_START,
    LEXER_STATE_SAW_MINUS,
    LEXER_STATE_SAW_ASSIGN,
    LEXER_STATE_SAW_TILDE,
    LEXER_STATE_SAW_LESS,
    LEXER_STATE_SAW_GREATER,
    LEXER_STATE_SAW_COLON,
    LEXER_STATE_SAW_DOT,
    LEXER_STATE_SAW_DOT_DOT,
    LEXER_STATE_SAW_SLASH,
    LEXER_STATE_COMMENT,
    LEXER_STATE_ALPHA,
    LEXER_STATE_NUMBER,
    LEXER_STATE_NUMBER_DOT,
    LEXER_STATE_SINGLE_QUOTED_STRING,
    LEXER_STATE_DOUBLE_QUOTED_STRING,
    LEXER_STATE_ERROR,
    LEXER_STATE_FINISH
} lua_lexer_state;

typedef struct {
    lua_lexer_state state;
    char *start;
    char *current;
    int line;
    int column;
} lua_lexer;

void lua_init_lexer(lua_lexer *lexer, const char *source);

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
    int column;
} lua_token;

lua_token lua_next_token(lua_lexer *lexer);

typedef struct {
    const char *keyword;
    lua_token_type token_type;
} lua_keyword;

#define LEXER_ALPHA \
         '_': \
    case 'a': \
    case 'b': \
    case 'c': \
    case 'd': \
    case 'e': \
    case 'f': \
    case 'g': \
    case 'h': \
    case 'i': \
    case 'j': \
    case 'k': \
    case 'l': \
    case 'm': \
    case 'n': \
    case 'o': \
    case 'p': \
    case 'q': \
    case 'r': \
    case 's': \
    case 't': \
    case 'u': \
    case 'v': \
    case 'w': \
    case 'x': \
    case 'y': \
    case 'z': \
    case 'A': \
    case 'B': \
    case 'C': \
    case 'D': \
    case 'E': \
    case 'F': \
    case 'G': \
    case 'H': \
    case 'I': \
    case 'J': \
    case 'K': \
    case 'L': \
    case 'M': \
    case 'N': \
    case 'O': \
    case 'P': \
    case 'Q': \
    case 'R': \
    case 'S': \
    case 'T': \
    case 'U': \
    case 'V': \
    case 'W': \
    case 'X': \
    case 'Y': \
    case 'Z'

#define LEXER_DIGIT \
         '0': \
    case '1': \
    case '2': \
    case '3': \
    case '4': \
    case '5': \
    case '6': \
    case '7': \
    case '8': \
    case '9'

#define WHITESPACE \
          ' ':  \
    case '\n': \
    case '\t': \
    case '\r'

#endif
