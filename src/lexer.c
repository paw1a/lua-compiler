#include "lexer.h"

#include <string.h>

void initialize_lexer(lua_lexer *lexer, char *source) {
    lexer->state = TOKEN_STATE_START;
    lexer->start = source;
    lexer->current = source;
    lexer->line = 0;
    lexer->column = 0;
}

static void error_token(lua_lexer *lexer, lua_token *token, const char *message) {
    token->type = TOKEN_ERROR;
    token->start = message;
    token->len = strlen(message);
    token->line = lexer->line;
    token->column = lexer->column;
}

static void begin_token(lua_lexer *lexer, lua_token *token, lua_token_type type) {
    token->type = type;
    token->start = lexer->current;
    token->len = 0;
    token->line = lexer->line;
    token->column = lexer->column;
}

lua_token next_token(lua_lexer *lexer) {
    lua_token token = { 0 };

    while (*lexer->current) {
        char c = *lexer->current;
        switch (lexer->state) {
            case TOKEN_STATE_START:
                switch (c) {
                    case '+':
                        begin_token(lexer, &token, TOKEN_PLUS);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '-':
                        begin_token(lexer, &token, TOKEN_MINUS);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '*':
                        begin_token(lexer, &token, TOKEN_STAR);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '%':
                        begin_token(lexer, &token, TOKEN_PERCENT);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '^':
                        begin_token(lexer, &token, TOKEN_EXP);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '#':
                        begin_token(lexer, &token, TOKEN_HASH);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '(':
                        begin_token(lexer, &token, TOKEN_LEFT_PAREN);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case ')':
                        begin_token(lexer, &token, TOKEN_RIGHT_PAREN);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '{':
                        begin_token(lexer, &token, TOKEN_LEFT_BRACE);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '}':
                        begin_token(lexer, &token, TOKEN_RIGHT_BRACE);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '[':
                        begin_token(lexer, &token, TOKEN_LEFT_BRACKET);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case ']':
                        begin_token(lexer, &token, TOKEN_RIGHT_BRACKET);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case ';':
                        begin_token(lexer, &token, TOKEN_SEMICOLON);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case ',':
                        begin_token(lexer, &token, TOKEN_COMMA);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '&':
                        begin_token(lexer, &token, TOKEN_AMPERSAND);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case '|':
                        begin_token(lexer, &token, TOKEN_PIPE);
                        lexer->state = TOKEN_STATE_START;
                        break;
                    case LEXER_ALPHA:
                        begin_token(lexer, &token, TOKEN_IDENTIFIER);
                        lexer->state = TOKEN_STATE_ALPHA;
                        break;
                    case LEXER_DIGIT:
                        break;
                    default:
                        break;
                }
                break;
            case TOKEN_STATE_SAW_ASSIGN:
                break;
            case TOKEN_STATE_SAW_TILDA:
                break;
            case TOKEN_STATE_SAW_LESS:
                break;
            case TOKEN_STATE_SAW_GREATER:
                break;
            case TOKEN_STATE_SAW_COLON:
                break;
            case TOKEN_STATE_SAW_DOT:
                break;
            case TOKEN_STATE_SAW_DOT_DOT:
                break;
            case TOKEN_STATE_SAW_SLASH:
                break;
            case TOKEN_STATE_ALPHA:
                break;
            case TOKEN_STATE_NUMBER:
                break;
            case TOKEN_STATE_NUMBER_DOT:
                break;
        }
    }

    return token;
}
