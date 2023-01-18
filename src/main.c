#include "vm.h"
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

    char *filename = argv[1];
    char *source = read_file(filename);
    if (source == NULL)
        return 1;

    debug_print_tokens(source);

    lua_vm vm;
    lua_init_vm(&vm);

    if (lua_interpret(&vm, source))
        return 1;

    free(source);
    lua_free_vm(&vm);

    return 0;
}
