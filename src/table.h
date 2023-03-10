#ifndef TABLE_H
#define TABLE_H

#include "value.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LOAD_FACTOR 0.75
#define TABLE_INIT_SIZE 8
#define MAX_BITS 32

typedef struct {
    lua_object key;
    lua_object value;
    bool tombstone;
} lua_table_entry;

typedef struct {
    size_t size;
    size_t capacity;
    lua_table_entry *entries;

    size_t array_size;
    size_t array_capacity;
    lua_object *array;
} lua_table;

void lua_table_init(lua_table *table);

bool lua_table_add(lua_table *table, lua_object key, lua_object value);
bool lua_table_delete(lua_table *table, lua_object key);
lua_object lua_table_find(lua_table *table, lua_object key);
bool lua_table_contains(lua_table *table, lua_object key);
bool lua_table_contains_string(lua_table *table, lua_string *str);

void lua_table_free(lua_table *table);

#endif
