#include "lexer.h"

#include <string.h>
#include <stdio.h>

void initialize_lexer(lua_lexer *lexer, char *source) {
    lexer->state = LEXER_STATE_START;
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

static void create_token(lua_lexer *lexer, lua_token *token, lua_token_type type) {
    token->type = type;
    token->start = lexer->current;
    token->len = 0;
    token->line = lexer->line;
    token->column = lexer->column;
}

static const lua_keyword keywords[] = {
        {"and", TOKEN_AND},
        {"break", TOKEN_BREAK},
        {"do", TOKEN_DO},
        {"else", TOKEN_ELSE},
        {"elseif", TOKEN_ELSEIF},
        {"end", TOKEN_END},
        {"false", TOKEN_FALSE},
        {"for", TOKEN_FOR},
        {"function", TOKEN_FUNCTION},
        {"goto", TOKEN_GOTO},
        {"if", TOKEN_IF},
        {"in", TOKEN_IN},
        {"local", TOKEN_LOCAL},
        {"nil", TOKEN_NIL},
        {"not", TOKEN_NOT},
        {"or", TOKEN_OR},
        {"repeat", TOKEN_REPEAT},
        {"return", TOKEN_RETURN},
        {"then", TOKEN_THEN},
        {"true", TOKEN_TRUE},
        {"until", TOKEN_UNTIL},
        {"while", TOKEN_WHILE},
};

static lua_token_type get_identifier_type(const char *identifier, size_t len) {
    size_t keywords_size = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < keywords_size; i++)
        if (strncmp(identifier, keywords[i].keyword, len) == 0)
            return keywords[i].token_type;

    return TOKEN_IDENTIFIER;
}

lua_token next_token(lua_lexer *lexer) {
    lua_token token = { 0 };
    lexer->state = LEXER_STATE_START;

    while (*lexer->current) {
        char c = *lexer->current;
        switch (lexer->state) {
            case LEXER_STATE_START:
                switch (c) {
                    case '+':
                        create_token(lexer, &token, TOKEN_PLUS);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '*':
                        create_token(lexer, &token, TOKEN_STAR);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '%':
                        create_token(lexer, &token, TOKEN_PERCENT);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '^':
                        create_token(lexer, &token, TOKEN_EXP);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '#':
                        create_token(lexer, &token, TOKEN_HASH);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '(':
                        create_token(lexer, &token, TOKEN_LEFT_PAREN);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case ')':
                        create_token(lexer, &token, TOKEN_RIGHT_PAREN);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '{':
                        create_token(lexer, &token, TOKEN_LEFT_BRACE);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '}':
                        create_token(lexer, &token, TOKEN_RIGHT_BRACE);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '[':
                        create_token(lexer, &token, TOKEN_LEFT_BRACKET);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case ']':
                        create_token(lexer, &token, TOKEN_RIGHT_BRACKET);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case ';':
                        create_token(lexer, &token, TOKEN_SEMICOLON);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case ',':
                        create_token(lexer, &token, TOKEN_COMMA);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '&':
                        create_token(lexer, &token, TOKEN_AMPERSAND);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '|':
                        create_token(lexer, &token, TOKEN_PIPE);
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                    case '=':
                        create_token(lexer, &token, TOKEN_ASSIGN);
                        lexer->state = LEXER_STATE_SAW_ASSIGN;
                        break;
                    case '-':
                        create_token(lexer, &token, TOKEN_MINUS);
                        lexer->state = LEXER_STATE_SAW_MINUS;
                        break;
                    case '~':
                        create_token(lexer, &token, TOKEN_TILDE);
                        lexer->state = LEXER_STATE_SAW_TILDE;
                        break;
                    case '<':
                        create_token(lexer, &token, TOKEN_LESS);
                        lexer->state = LEXER_STATE_SAW_LESS;
                        break;
                    case '>':
                        create_token(lexer, &token, TOKEN_GREATER);
                        lexer->state = LEXER_STATE_SAW_GREATER;
                        break;
                    case ':':
                        create_token(lexer, &token, TOKEN_COLON);
                        lexer->state = LEXER_STATE_SAW_COLON;
                        break;
                    case '.':
                        create_token(lexer, &token, TOKEN_DOT);
                        lexer->state = LEXER_STATE_SAW_DOT;
                        break;
                    case '/':
                        create_token(lexer, &token, TOKEN_SLASH);
                        lexer->state = LEXER_STATE_SAW_SLASH;
                        break;
                    case '\'':
                        create_token(lexer, &token, TOKEN_STRING);
                        lexer->state = LEXER_STATE_SINGLE_QUOTED_STRING;
                        break;
                    case '\"':
                        create_token(lexer, &token, TOKEN_STRING);
                        lexer->state = LEXER_STATE_DOUBLE_QUOTED_STRING;
                        break;
                    case LEXER_ALPHA:
                        create_token(lexer, &token, TOKEN_IDENTIFIER);
                        lexer->state = LEXER_STATE_ALPHA;
                        break;
                    case LEXER_DIGIT:
                        create_token(lexer, &token, TOKEN_NUMBER);
                        lexer->state = LEXER_STATE_NUMBER;
                        break;
                    case WHITESPACE:
                        break;
                    default:
                        error_token(lexer, &token, "unexpected token");
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                }
                break;
            case LEXER_STATE_SAW_ASSIGN:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case '=':
                        token.type = TOKEN_EQUAL;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_MINUS:
                switch (c) {
                    case '-':
                        lexer->state = LEXER_STATE_COMMENT;
                        break;
                    default:
                        lexer->state = LEXER_STATE_FINISH;
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_COMMENT:
                switch (c) {
                    case '\n':
                        lexer->state = LEXER_STATE_START;
                        break;
                    default:
                        break;
                }
                break;
            case LEXER_STATE_SAW_TILDE:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case '=':
                        token.type = TOKEN_NOT_EQUAL;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_LESS:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case '=':
                        token.type = TOKEN_LESS_EQUAL;
                        break;
                    case '<':
                        token.type = TOKEN_SHIFT_LEFT;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_GREATER:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case '=':
                        token.type = TOKEN_GREATER_EQUAL;
                        break;
                    case '>':
                        token.type = TOKEN_SHIFT_RIGHT;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_COLON:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case ':':
                        token.type = TOKEN_DOUBLE_COLON;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_DOT:
                switch (c) {
                    case '.':
                        lexer->state = LEXER_STATE_SAW_DOT_DOT;
                        token.type = TOKEN_DOUBLE_DOT;
                        break;
                    default:
                        lexer->state = LEXER_STATE_FINISH;
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_DOT_DOT:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case '.':
                        token.type = TOKEN_TRIPLE_DOT;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_SAW_SLASH:
                lexer->state = LEXER_STATE_FINISH;
                switch (c) {
                    case '/':
                        token.type = TOKEN_DOUBLE_SLASH;
                        break;
                    default:
                        lexer->current--;
                        break;
                }
                break;
            case LEXER_STATE_ALPHA:
                switch (c) {
                    case LEXER_DIGIT:
                    case LEXER_ALPHA:
                        break;
                    default:
                        token.type = get_identifier_type(token.start, lexer->current - token.start);
                        lexer->current--;
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                }
                break;
            case LEXER_STATE_NUMBER:
                switch (c) {
                    case LEXER_DIGIT:
                        break;
                    case '.':
                        lexer->state = LEXER_STATE_NUMBER_DOT;
                        break;
                    default:
                        lexer->current--;
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                }
                break;
            case LEXER_STATE_NUMBER_DOT:
                switch (c) {
                    case LEXER_DIGIT:
                        break;
                    default:
                        lexer->current--;
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                }
                break;
            case LEXER_STATE_SINGLE_QUOTED_STRING:
                switch (c) {
                    case '\'':
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                }
                break;
            case LEXER_STATE_DOUBLE_QUOTED_STRING:
                switch (c) {
                    case '\"':
                        lexer->state = LEXER_STATE_FINISH;
                        break;
                }
                break;
            case LEXER_STATE_ERROR:
                break;
        }

        lexer->current++;
        if (lexer->state == LEXER_STATE_FINISH) {
            token.len = lexer->current - token.start;
            return token;
        }

        if (c == '\n') {
            lexer->line++;
            lexer->column = 0;
        } else {
            lexer->column++;
        }
    }

    switch (lexer->state) {
        case LEXER_STATE_COMMENT:
        case LEXER_STATE_START:
            create_token(lexer, &token, TOKEN_EOF);
            break;
        case LEXER_STATE_DOUBLE_QUOTED_STRING:
        case LEXER_STATE_SINGLE_QUOTED_STRING:
            error_token(lexer, &token, "unterminated string");
            break;
        case LEXER_STATE_SAW_ASSIGN:
        case LEXER_STATE_SAW_TILDE:
        case LEXER_STATE_SAW_LESS:
        case LEXER_STATE_SAW_GREATER:
        case LEXER_STATE_SAW_COLON:
        case LEXER_STATE_SAW_DOT:
        case LEXER_STATE_SAW_DOT_DOT:
        case LEXER_STATE_SAW_SLASH:
        case LEXER_STATE_NUMBER:
        case LEXER_STATE_NUMBER_DOT:
            token.len = lexer->current - token.start;
            break;
        case LEXER_STATE_ALPHA:
            token.type = get_identifier_type(token.start, lexer->current - token.start);
            token.len = lexer->current - token.start;
            break;
        case LEXER_STATE_ERROR:
        case LEXER_STATE_FINISH:
            break;
    }

    return token;
}
