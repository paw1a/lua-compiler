#include "bytecode.h"
#include "memory.h"

#include <stdlib.h>

void lua_init_bytecode(lua_bytecode *bytecode) {
    bytecode->opcodes = NULL;
    bytecode->constants = NULL;
}

void lua_bytecode_emit_uint8(lua_bytecode *bytecode, uint8_t data) {
    sb_push(bytecode->opcodes, data);
}

void lua_bytecode_write_uint16(lua_bytecode *bytecode, uint16_t data) {
    lua_bytecode_emit_uint8(bytecode, (data >> 8) & 0xff);
    lua_bytecode_emit_uint8(bytecode, data & 0xff);
}

void lua_bytecode_write_uint32(lua_bytecode *bytecode, uint32_t data) {
    lua_bytecode_emit_uint8(bytecode, (data >> 24) & 0xff);
    lua_bytecode_emit_uint8(bytecode, (data >> 16) & 0xff);
    lua_bytecode_emit_uint8(bytecode, (data >> 8) & 0xff);
    lua_bytecode_emit_uint8(bytecode, data & 0xff);
}

uint32_t lua_bytecode_emit_opcode(lua_bytecode *bytecode, lua_opcode opcode) {
    lua_bytecode_emit_uint8(bytecode, opcode);

    // returns offset in the array of opcodes
    return sb_size(bytecode->opcodes) - 1;
}

uint32_t lua_bytecode_emit_constant(lua_bytecode *bytecode, lua_object constant) {
    sb_push(bytecode->constants, constant);

    // returns offset in the array of constants
    return sb_size(bytecode->constants) - 1;
}

void lua_free_bytecode(lua_bytecode *bytecode) {
    sb_free(bytecode->opcodes);
    bytecode->opcodes = NULL;
}
