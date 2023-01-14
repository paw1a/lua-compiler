#include "compiler.h"

#include <stdio.h>

void lua_init_compiler(lua_compiler *compiler) {
    compiler->lexer = NULL;
    compiler->bytecode = NULL;
}

static void advance(lua_compiler *compiler) {
    compiler->previous = compiler->current;

    for (;;) {
        compiler->current = lua_next_token(compiler->lexer);
        if (compiler->current.type != TOKEN_ERROR)
            break;

        // TODO: error handling
        exit(4);
    }
}

static void consume(lua_compiler *compiler,
                    lua_token_type token_type, const char *message) {
    if (compiler->current.type == token_type) {
        advance(compiler);
        return;
    }

    // TODO: error handling
    exit(3);
}

typedef enum {
    PRECEDENCE_NONE,        // the lowest precedence
    PRECEDENCE_ASSIGNMENT,  // =
    PRECEDENCE_OR,          // or
    PRECEDENCE_AND,         // and
    PRECEDENCE_EQUALITY,    // == !=
    PRECEDENCE_COMPARISON,  // < > <= >=
    PRECEDENCE_TERM,        // + -
    PRECEDENCE_FACTOR,      // * /
    PRECEDENCE_UNARY,       // ! -
    PRECEDENCE_CALL,        // . ()
    PRECEDENCE_PRIMARY      // literals, the highest precedence
} lua_precedence;

typedef void (*lua_compile_func) (lua_compiler *compiler);

typedef struct {
    lua_compile_func prefix;
    lua_compile_func infix;
    lua_precedence precedence;
} lua_parse_rule;

static void compile_number(lua_compiler *compiler);
static void compile_grouping(lua_compiler *compiler);
static void compile_unary(lua_compiler *compiler);
static void compile_binary(lua_compiler *compiler);

lua_parse_rule parse_rules[] = {
        [TOKEN_LEFT_PAREN]    = {compile_grouping, NULL,           PRECEDENCE_NONE},
        [TOKEN_RIGHT_PAREN]   = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_LEFT_BRACE]    = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_RIGHT_BRACE]   = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_COMMA]         = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_DOT]           = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_MINUS]         = {compile_unary,    compile_binary, PRECEDENCE_TERM},
        [TOKEN_PLUS]          = {NULL,             compile_binary, PRECEDENCE_TERM},
        [TOKEN_SEMICOLON]     = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_SLASH]         = {NULL,             compile_binary, PRECEDENCE_FACTOR},
        [TOKEN_STAR]          = {NULL,             compile_binary, PRECEDENCE_FACTOR},
        [TOKEN_NOT]           = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_NOT_EQUAL]     = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_EQUAL]         = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_GREATER]       = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_GREATER_EQUAL] = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_LESS]          = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_LESS_EQUAL]    = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_IDENTIFIER]    = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_STRING]        = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_NUMBER]        = {compile_number,   NULL,           PRECEDENCE_NONE},
        [TOKEN_AND]           = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_ELSE]          = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_FALSE]         = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_FOR]           = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_IF]            = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_NIL]           = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_OR]            = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_RETURN]        = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_TRUE]          = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_WHILE]         = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_ERROR]         = {NULL,             NULL,           PRECEDENCE_NONE},
        [TOKEN_EOF]           = {NULL,             NULL,           PRECEDENCE_NONE},
};

static lua_parse_rule get_rule(lua_token_type token_type) {
    return parse_rules[token_type];
}

static void parse_precedence(lua_compiler *compiler, lua_precedence precedence) {
    advance(compiler);
    lua_compile_func prefix_func = get_rule(compiler->previous.type).prefix;
    if (prefix_func == NULL) {
        // TODO: handle error
        exit(2);
    }

    prefix_func(compiler);

    while (get_rule(compiler->current.type).precedence >= precedence) {
        advance(compiler);
        lua_compile_func infix_func = get_rule(compiler->previous.type).infix;
        infix_func(compiler);
    }
}

static void compile_expression(lua_compiler *compiler) {
    parse_precedence(compiler, PRECEDENCE_ASSIGNMENT);
}

static void compile_number(lua_compiler *compiler) {
    lua_number number = strtod(compiler->previous.start, NULL);
    lua_object number_obj = lua_create_number(number);
    lua_bytecode_emit_constant(compiler->bytecode, number_obj);
}

static void compile_grouping(lua_compiler *compiler) {
    compile_expression(compiler);
    consume(compiler, TOKEN_RIGHT_PAREN, "expected ')' token");
}

static void compile_unary(lua_compiler *compiler) {
    lua_token_type operator = compiler->previous.type;

    parse_precedence(compiler, PRECEDENCE_UNARY);

    switch (operator) {
        case TOKEN_MINUS:
            lua_bytecode_emit_opcode(compiler->bytecode, OP_NEGATE);
            break;
        default: return;
    }
}

static void compile_binary(lua_compiler *compiler) {
    lua_token_type operator = compiler->previous.type;
    lua_parse_rule infix_rule = get_rule(operator);

    parse_precedence(compiler, (infix_rule.precedence + 1));

    switch (operator) {
        case TOKEN_PLUS:
            lua_bytecode_emit_opcode(compiler->bytecode, OP_ADD);
            break;
        case TOKEN_MINUS:
            lua_bytecode_emit_opcode(compiler->bytecode, OP_NEGATE);
            break;
        case TOKEN_STAR:
            lua_bytecode_emit_opcode(compiler->bytecode, OP_MULTIPLY);
            break;
        case TOKEN_SLASH:
            lua_bytecode_emit_opcode(compiler->bytecode, OP_DIVIDE);
            break;
        default: return;
    }
}

bool lua_compile(lua_compiler *compiler, const char *source, lua_bytecode *bytecode) {
    lua_lexer lexer;
    lua_init_lexer(&lexer, source);

    compiler->lexer = &lexer;
    compiler->bytecode = bytecode;

    advance(compiler);
    compile_expression(compiler);
    consume(compiler, TOKEN_EOF, "expected end of file");

    return true;
}
