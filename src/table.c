#include "table.h"
#include "value.h"
#include "memory.h"
#include "util.h"

#include <string.h>
#include <stdbool.h>

void lua_table_init(lua_table *table) {
    table->entries = NULL;
    table->array = NULL;
    table->capacity = 0;
    table->size = 0;
    table->array_size = 0;
    table->array_capacity = 0;
}

static void count_int_keys(lua_table *table, uint32_t *int_keys) {
    for (size_t i = 0; i < table->capacity; i++) {
        lua_table_entry entry = table->entries[i];
        if (lua_is_integer(entry.key) && entry.key.num > 0)
            int_keys[log2_32((uint32_t) entry.key.num)]++;
    }

    for (size_t i = 0; i < table->array_capacity; i++) {
        lua_object obj = table->array[i];
        if (!lua_is_nil(obj))
            int_keys[log2_32((uint32_t) i)]++;
    }
}

static bool table_rehash(lua_table *table) {
    if (table->entries == NULL) {
        table->entries = lua_calloc(TABLE_INIT_SIZE, sizeof(lua_table_entry));
        if (table->entries == NULL)
            return false;
        table->capacity = TABLE_INIT_SIZE;
        return true;
    }

    uint32_t int_keys[MAX_BITS];
    memset(int_keys, 0, MAX_BITS);
    count_int_keys(table, int_keys);

    bool array_expanded = false;
    size_t half_total_size = (table->size + table->array_size) / 2;
    for (size_t i = 0; i < MAX_BITS; i++) {
        if (int_keys[i] >= half_total_size) {
            table->array = lua_realloc(table->array, table->array_capacity, 1 << i);
            if (table->array == NULL)
                return false;
            table->array_capacity = 1 << i;
            array_expanded = true;
            break;
        }
    }

    lua_table old_table = *table;

    if (!array_expanded) {
        table->capacity *= 2;
        table->entries = lua_calloc(table->capacity, sizeof(lua_table_entry));
        if (table->entries == NULL) {
            lua_free(old_table.entries);
            return false;
        }
    }

    for (size_t i = 0; i < old_table.capacity; i++) {
        lua_table_entry *entry = &old_table.entries[i];
        if (!lua_is_equal(entry->key, lua_nil))
            if (!lua_table_add(table, entry->key, entry->value)) {
                lua_free(old_table.entries);
                return false;
            }
    }

    if (!array_expanded)
        lua_free(old_table.entries);

    return true;
}

bool lua_table_add(lua_table *table, lua_object key, lua_object value) {
    if (lua_is_integer(key) && key.num > 0 && key.num < table->array_capacity) {
        table->array[(uint32_t) key.num] = value;
        table->array_size++;
        return true;
    }

    if (table->size + 1 > table->capacity * MAX_LOAD_FACTOR)
        if (!table_rehash(table))
            return false;

    uint32_t hash = lua_hash_object(key);
    size_t index = hash % table->capacity;

    for (size_t i = 0; i < table->capacity; i++) {
        lua_table_entry *entry = &table->entries[index];
        if (lua_is_equal(entry->key, lua_nil)) {
            entry->key = key;
            entry->value = value;
            break;
        }

        if (lua_is_equal(key, entry->key)) {
            entry->value = value;
            break;
        }

        // index = (index + 1) % table.capacity;
        index = ~table->capacity & (index + 1);
    }

    table->size++;

    return true;
}

lua_object lua_table_find(lua_table *table, lua_object key) {
    if (lua_is_integer(key) && key.num > 0 && key.num < table->array_capacity) {
        return table->array[(uint32_t) key.num];
    }

    uint32_t hash = lua_hash_object(key);
    size_t index = hash % table->capacity;

    for (size_t i = 0; i < table->capacity; i++) {
        lua_table_entry *entry = &table->entries[index];
        if (lua_is_equal(entry->key, lua_nil))
            break;

        if (!entry->tombstone && lua_is_equal(key, entry->key))
            return entry->value;

        // index = (index + 1) % table.capacity;
        index = ~table->capacity & (index + 1);
    }

    return lua_nil;
}

bool lua_table_contains(lua_table *table, lua_object key) {
    if (lua_is_integer(key) && key.num > 0 && key.num < table->array_capacity) {
        return !lua_is_nil(table->array[(uint32_t) key.num]);
    }

    uint32_t hash = lua_hash_object(key);
    size_t index = hash % table->capacity;

    for (size_t i = 0; i < table->capacity; i++) {
        lua_table_entry *entry = &table->entries[index];
        if (lua_is_equal(entry->key, lua_nil))
            break;

        if (!entry->tombstone && lua_is_equal(key, entry->key))
            return true;

        // index = (index + 1) % table.capacity;
        index = ~table->capacity & (index + 1);
    }

    return false;
}

bool lua_table_contains_string(lua_table *table, lua_string *str) {
    uint32_t hash = lua_hash_string(str);
    size_t index = hash % table->capacity;

    for (size_t i = 0; i < table->capacity; i++) {
        lua_table_entry *entry = &table->entries[index];
        if (lua_is_equal(entry->key, lua_nil))
            break;

        lua_string *key_string = (lua_string *) entry->key.gc_obj;
        if (!entry->tombstone &&
                key_string->len == str->len &&
                key_string->hash == str->hash &&
                memcmp(key_string->chars, str->chars, str->len) == 0)
            return true;

        // index = (index + 1) % table.capacity;
        index = ~table->capacity & (index + 1);
    }

    return false;
}

bool lua_table_delete(lua_table *table, lua_object key) {
    if (lua_is_integer(key) && key.num > 0 && key.num < table->array_capacity) {
        table->array[(uint32_t) key.num] = lua_nil;
        table->array_size--;
        return true;
    }

    uint32_t hash = lua_hash_object(key);
    size_t index = hash % table->capacity;

    for (size_t i = 0; i < table->capacity; i++) {
        lua_table_entry *entry = &table->entries[index];
        if (lua_is_equal(entry->key, lua_nil))
            return false;

        if (!entry->tombstone && lua_is_equal(key, entry->key)) {
            // TODO: free deleted object
            table->size--;
            table->entries[index].tombstone = true;
            break;
        }

        // index = (index + 1) % table.capacity;
        index = ~table->capacity & (index + 1);
    }

    return true;
}

void lua_table_free(lua_table *table) {
    lua_free(table->entries);
    lua_free(table->array);
}
