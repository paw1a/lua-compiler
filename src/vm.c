#include "vm.h"
#include "common.h"
#include "debug.h"

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
        }
    }
}

lua_interpret_result interpret(lua_vm *vm, lua_bytecode *bytecode) {
    vm->bytecode = bytecode;
    vm->ip = (uint8_t *) vm->bytecode;
    return run(vm);
}

void lua_push(lua_vm *vm, lua_object value) {
    *vm->sp++ = value;
}

lua_object lua_pop(lua_vm *vm) {
    return *vm->sp--;
}

void lua_free_vm(lua_vm *vm) {

}
