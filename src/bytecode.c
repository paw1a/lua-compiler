#include "bytecode.h"
#include "memory.h"

#include <stdlib.h>

void lua_init_bytecode(lua_bytecode *bytecode) {
    bytecode->opcodes = NULL;
    bytecode->constants = NULL;
}

void lua_bytecode_add_opcode(lua_bytecode *bytecode, lua_opcode opcode) {
    sb_push(bytecode->opcodes, opcode);
}

void lua_bytecode_add_uint8(lua_bytecode *bytecode, uint8_t data) {
    sb_push(bytecode->opcodes, data);
}

uint32_t lua_bytecode_add_constant(lua_bytecode *bytecode, lua_object constant) {
    sb_push(bytecode->constants, constant);

    // returns offset in the array of constants
    return sb_size(bytecode->constants) - 1;
}

void lua_free_bytecode(lua_bytecode *bytecode) {
    sb_free(bytecode->opcodes);
    bytecode->opcodes = NULL;
}
