#include "bytecode.h"
#include "memory.h"

#include <stdlib.h>

void lua_init_bytecode(lua_bytecode *bytecode) {
    bytecode->opcodes = NULL;
}

void lua_add_opcode(lua_bytecode *bytecode, lua_opcode opcode) {
    sb_push(bytecode->opcodes, opcode);
}

void lua_free_bytecode(lua_bytecode *bytecode) {
    sb_free(bytecode->opcodes);
    bytecode->opcodes = NULL;
}
