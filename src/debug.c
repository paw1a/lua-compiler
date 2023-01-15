#include "debug.h"
#include "memory.h"
#include "bytecode.h"
#include "lexer.h"

#include <stdio.h>

void debug_disassemble_bytecode(lua_bytecode *bytecode, const char *name) {
    printf("=== %s ===\n", name);
    for (uint32_t offset = 0; offset < sb_size(bytecode->opcodes);) {
        offset = debug_disassemble_instruction(bytecode, offset);
    }
}

static uint32_t debug_simple_instruction(const char *opcode_name, uint32_t offset) {
    printf("%s\n", opcode_name);
    return offset + 1;
}

static uint32_t debug_constant_instruction(const char *opcode_name,
                                           lua_bytecode *bytecode, uint32_t offset) {
    uint8_t constant_offset = bytecode->opcodes[offset + 1];
    printf("%-16s %u '", opcode_name, constant_offset);
    lua_object obj = bytecode->constants[constant_offset];
    lua_print_object(obj);
    printf("'\n");
    return offset + 2;
}

uint32_t debug_disassemble_instruction(lua_bytecode *bytecode, uint32_t offset) {
    printf("%04u ", offset);
    uint8_t opcode = bytecode->opcodes[offset];
    switch (opcode) {
        case OP_RETURN:
            return debug_simple_instruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return debug_constant_instruction("OP_CONSTANT", bytecode, offset);
        case OP_NIL:
            return debug_simple_instruction("OP_NIL", offset);
        case OP_TRUE:
            return debug_simple_instruction("OP_TRUE", offset);
        case OP_FALSE:
            return debug_simple_instruction("OP_FALSE", offset);
        case OP_NEGATE:
            return debug_simple_instruction("OP_NEGATE", offset);
        case OP_ADD:
            return debug_simple_instruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return debug_simple_instruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return debug_simple_instruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return debug_simple_instruction("OP_DIVIDE", offset);
        case OP_NOT:
            return debug_simple_instruction("OP_NOT", offset);
        case OP_LT:
            return debug_simple_instruction("OP_LT", offset);
        case OP_LE:
            return debug_simple_instruction("OP_LE", offset);
        case OP_EQ:
            return debug_simple_instruction("OP_EQ", offset);
        default:
            printf("unknown opcode %d\n", opcode);
            return offset + 1;
    }
}

void debug_print_tokens(const char *source) {
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
