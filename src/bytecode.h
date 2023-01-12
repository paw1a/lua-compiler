#ifndef BYTECODE_H
#define BYTECODE_H

#include "value.h"

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    OP_RETURN,
    OP_CONSTANT
} lua_opcode;

typedef struct {
    // stretchy buffer of opcodes
    uint8_t *opcodes;
    // stretchy buffer of lua constants
    lua_object *constants;
    // stretchy buffer of line numbers
    uint32_t *lines;
} lua_bytecode;

void lua_init_bytecode(lua_bytecode *bytecode);

void lua_bytecode_write_uint8(lua_bytecode *bytecode, uint8_t data);
void lua_bytecode_write_uint16(lua_bytecode *bytecode, uint16_t data);
void lua_bytecode_write_uint32(lua_bytecode *bytecode, uint32_t data);

void lua_bytecode_write_opcode(lua_bytecode *bytecode, lua_opcode opcode);
uint32_t lua_bytecode_add_constant(lua_bytecode *bytecode, lua_object constant);

void lua_free_bytecode(lua_bytecode *bytecode);

#endif
