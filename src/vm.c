#include "vm.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"

void lua_init_vm(lua_vm *vm) {
    vm->bytecode = NULL;
    vm->ip = NULL;
    vm->sp = vm->stack;
}

static lua_interpret_result run(lua_vm *vm) {
    for (;;) {
#ifdef DEBUG
        debug_disassemble_instruction(vm->bytecode,
                                      vm->ip - vm->bytecode->opcodes);
        printf("          ");
        for (lua_object *cursor = vm->stack; cursor < vm->sp; cursor++) {
            printf("[ ");
            lua_print_object(*cursor);
            printf(" ]");
        }
        printf("\n");
#endif

        uint8_t instruction = *vm->ip++;
        switch (instruction) {
            case OP_RETURN:
                lua_print_object(lua_pop(vm));
                printf("\n");
                return INTERPRET_OK;
            case OP_CONSTANT: {
                lua_object constant = vm->bytecode->constants[*vm->ip++];
                lua_push(vm, constant);
                break;
            }
            case OP_NEGATE: lua_push(vm, lua_pop(vm)); break;
            case OP_ADD: break;
            case OP_SUBTRACT: break;
            case OP_MULTIPLY: break;
            case OP_DIVIDE: break;
        }
    }
}

lua_interpret_result lua_interpret(lua_vm *vm, const char *source) {
    lua_bytecode bytecode;
    lua_init_bytecode(&bytecode);

    lua_compiler compiler;
    lua_init_compiler(&compiler);

    if (!lua_compile(&compiler, source, &bytecode)) {
        lua_free_bytecode(&bytecode);
        return INTERPRET_COMPILE_ERROR;
    }

    vm->bytecode = &bytecode;
    vm->ip = (uint8_t *) vm->bytecode;

    lua_interpret_result result = run(vm);

    lua_free_bytecode(&bytecode);
    return result;
}

void lua_push(lua_vm *vm, lua_object value) {
    *vm->sp++ = value;
}

lua_object lua_pop(lua_vm *vm) {
    return *vm->sp--;
}

void lua_free_vm(lua_vm *vm) {

}
