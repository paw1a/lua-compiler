#ifndef BYTECODE_H
#define BYTECODE_H

#include <stdint.h>

typedef enum {
    OP_RETURN,
} lua_opcode;

typedef struct {
    // stretchy buffer
    uint8_t *opcodes;
} lua_bytecode;

void lua_init_bytecode(lua_bytecode *bytecode);
void lua_add_opcode(lua_bytecode *bytecode, lua_opcode opcode);
void lua_free_bytecode(lua_bytecode *bytecode);

#endif
