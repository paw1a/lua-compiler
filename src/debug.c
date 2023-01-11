#include "debug.h"
#include "memory.h"
#include "bytecode.h"

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
    return offset + 1;
}

uint32_t debug_disassemble_instruction(lua_bytecode *bytecode, uint32_t offset) {
    printf("%04u ", offset);
    uint8_t opcode = bytecode->opcodes[offset];
    switch (opcode) {
        case OP_RETURN:
            return debug_simple_instruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return debug_constant_instruction("OP_CONSTANT", bytecode, offset);
        default:
            printf("unknown opcode %d\n", opcode);
            return offset + 1;
    }
}
