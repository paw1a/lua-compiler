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
        printf("STACK: ");
        for (lua_object *cursor = vm->stack; cursor < vm->sp; cursor++) {
            lua_print_object(*cursor);
            printf(" ");
        }
        printf("\n");

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
            case OP_NEGATE: {
                if (!lua_is_number(lua_peek(vm, 0))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number number = lua_get_number(lua_pop(vm));
                lua_push(vm, lua_create_number(-number));
                break;
            }
            case OP_ADD: {
                if (!lua_is_number(lua_peek(vm, 0)) || !lua_is_number(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number b = lua_get_number(lua_pop(vm));
                lua_number a = lua_get_number(lua_pop(vm));
                lua_push(vm, lua_create_number(a + b));
                break;
            }
            case OP_SUBTRACT: {
                if (!lua_is_number(lua_peek(vm, 0)) || !lua_is_number(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number b = lua_get_number(lua_pop(vm));
                lua_number a = lua_get_number(lua_pop(vm));
                lua_push(vm, lua_create_number(a - b));
                break;
            }
            case OP_MULTIPLY: {
                if (!lua_is_number(lua_peek(vm, 0)) || !lua_is_number(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number b = lua_get_number(lua_pop(vm));
                lua_number a = lua_get_number(lua_pop(vm));
                lua_push(vm, lua_create_number(a * b));
                break;
            }
            case OP_DIVIDE: {
                if (!lua_is_number(lua_peek(vm, 0)) || !lua_is_number(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number b = lua_get_number(lua_pop(vm));
                lua_number a = lua_get_number(lua_pop(vm));
                lua_push(vm, lua_create_number(a / b));
                break;
            }
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

    debug_disassemble_bytecode(&bytecode, "expression bytecode");

    vm->bytecode = &bytecode;
    vm->ip = (uint8_t *) vm->bytecode->opcodes;

    lua_interpret_result result = run(vm);

    lua_free_bytecode(&bytecode);
    return result;
}

void lua_push(lua_vm *vm, lua_object value) {
    *vm->sp = value;
    vm->sp++;
}

lua_object lua_pop(lua_vm *vm) {
    vm->sp--;
    return *vm->sp;
}

lua_object lua_peek(lua_vm *vm, size_t depth) {
    return vm->sp[-1 - depth];
}

void lua_free_vm(lua_vm *vm) {

}
