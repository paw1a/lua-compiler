#ifndef BYTECODE_H
#define BYTECODE_H

#include "value.h"

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    OP_RETURN,
} lua_opcode;

typedef struct {
    // stretchy buffer of opcodes
    uint8_t *opcodes;
    // stretchy buffer of lua constants
    lua_object *constants;
} lua_bytecode;

void lua_init_bytecode(lua_bytecode *bytecode);
void lua_bytecode_add_opcode(lua_bytecode *bytecode, lua_opcode opcode);
size_t lua_bytecode_add_constant(lua_bytecode *bytecode, lua_object constant);
void lua_free_bytecode(lua_bytecode *bytecode);

#endif
