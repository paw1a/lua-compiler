#include "vm.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "memory.h"
#include "table.h"

#include <string.h>

void lua_init_vm(lua_vm *vm) {
    vm->bytecode = NULL;
    vm->ip = NULL;
    vm->sp = vm->stack;
    vm->obj_list = NULL;
    lua_table_init(&vm->string_pool);
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
            case OP_NIL:
                lua_push(vm, lua_create_nil());
                break;
            case OP_TRUE:
                lua_push(vm, lua_create_bool(true));
                break;
            case OP_FALSE:
                lua_push(vm, lua_create_bool(false));
                break;
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
            case OP_NOT: {
                bool not_value = !lua_is_truthy(lua_pop(vm));
                lua_push(vm, lua_create_bool(not_value));
                break;
            }
            case OP_EQ: {
                lua_object b = lua_pop(vm);
                lua_object a = lua_pop(vm);
                bool is_equal = lua_is_equal(a, b);
                lua_push(vm, lua_create_bool(is_equal));
                break;
            }
            case OP_LT: {
                if (!lua_is_number(lua_peek(vm, 0)) || !lua_is_number(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number b = lua_get_number(lua_pop(vm));
                lua_number a = lua_get_number(lua_pop(vm));
                bool is_lt = a < b;
                lua_push(vm, lua_create_bool(is_lt));
                break;
            }
            case OP_LE: {
                if (!lua_is_number(lua_peek(vm, 0)) || !lua_is_number(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }
                lua_number b = lua_get_number(lua_pop(vm));
                lua_number a = lua_get_number(lua_pop(vm));
                bool is_lt = a <= b;
                lua_push(vm, lua_create_bool(is_lt));
                break;
            }
            case OP_CONCAT: {
                if (!lua_is_string(lua_peek(vm, 0)) || !lua_is_string(lua_peek(vm, 1))) {
                    // TODO: error handling
                    return INTERPRET_RUNTIME_ERROR;
                }

                lua_string *b = lua_get_string(lua_pop(vm));
                lua_string *a = lua_get_string(lua_pop(vm));

                size_t concat_len = a->len + b->len;

                lua_string *concat_str = alloc_string(concat_len);
                memcpy(concat_str->chars, a->chars, a->len);
                memcpy(concat_str->chars + a->len, b->chars, b->len);
                concat_str->len = concat_len;
                concat_str->chars[concat_len] = '\0';

                lua_push(vm, lua_create_gc_obj(
                        (lua_gc_object *) concat_str, VALUE_TYPE_STRING));
            }
        }
    }
}

lua_interpret_result lua_interpret(lua_vm *vm, const char *source) {
    lua_bytecode bytecode;
    lua_init_bytecode(&bytecode);

    lua_compiler compiler;
    lua_init_compiler(&compiler, vm);

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
    lua_gc_object *head = vm->obj_list;
    while (head != NULL) {
        lua_gc_object *next = head->next;
        lua_free(head);
        head = next;
    }
    lua_table_free(&vm->string_pool);
}
