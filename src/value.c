#include "value.h"

#include <stdio.h>

void lua_print_object(lua_object obj) {
    switch (obj.type) {
        case VALUE_TYPE_NUMBER:
            printf("%g", obj.value.num);
            break;
        case VALUE_TYPE_BOOL:
            printf("%d", obj.value.b);
            break;
        case VALUE_TYPE_STRING:
            printf("string");
            break;
    }
}
