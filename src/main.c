#include "compiler.h"
#include "lexer.h"
#include "memory.h"
#include "bytecode.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

static char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        return NULL;
    }

    rewind(file);

    char *buffer = malloc(size + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    fread(buffer, size, 1, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

int main(int argc, char **argv) {
    if (argc < 2)
        return 1;

    lua_bytecode bytecode;
    lua_init_bytecode(&bytecode);

    lua_add_opcode(&bytecode, OP_RETURN);
    lua_add_opcode(&bytecode, OP_RETURN);

    debug_disassemble_bytecode(&bytecode, "test");

    lua_free_bytecode(&bytecode);

    return 0;

    char *filename = argv[1];
    char *source = read_file(filename);
    if (source == NULL)
        return 1;

    lua_lexer lexer;
    initialize_lexer(&lexer, source);

    int line = -1;
    for (;;) {
        lua_token token = next_token(&lexer);
        if (token.line != line) {
            printf("%4d ", token.line + 1);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, (int)token.len, token.start);

        if (token.type == TOKEN_EOF)
            break;
    }

    free(source);

    return 0;
}
