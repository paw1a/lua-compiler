#ifndef DEBUG_H
#define DEBUG_H

#include "bytecode.h"
#include <stdio.h>

void debug_disassemble_bytecode(lua_bytecode *bytecode, const char *name);
uint32_t debug_disassemble_instruction(lua_bytecode *bytecode, uint32_t offset);

void debug_print_tokens(const char *source);

#endif
