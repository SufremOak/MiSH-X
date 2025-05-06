#include <stdlib.h>
#include <string.h>
#sts[i].count;
        }
    }

    return 0;
}

const char* c_get_global_list_item(const char* list_name, int index) {
    initialize_lists_if_needed();

    for (int i = 0; i < MAX_LISTS; i++) {
        if (global_lists[i].used && strcmp(global_lists[i].name, list_name) == 0) {
            if (index >= 0 && index < global_lists[i].count) {
                return global_lists[i].items[index];
            }
            break;
        }
    }

    return NULL;
}

void c_clear_globals() {
    // Clear key-value store
    if (initialized) {
        for (int i = 0; i < MAX_GLOBALS; i++) {
            global_values[i].used = 0;
        }
    }

    // Clear lists
    if (lists_initialized) {
        for (int i = 0; i < MAX_LISTS; i++) {
            global_lists[i].used = 0;
            global_lists[i].count = 0;
        }
    }
}

/* ---- Counter implementation ---- */
Counter* create_counter(int initial_value) {
    Counter* counter = (Counter*)malloc(sizeof(Counter));
    if (counter) {
        counter->value = initial_value;
    }
    return counter;
}

void increment_counter(Counter* counter) {
    if (counter) {
        counter->value++;
    }
}

int get_counter_value(const Counter* counter) {
    return counter ? counter->value : 0;
}

void destroy_counter(Counter* counter) {
    free(counter);
}include <stdio.h>
#include "core.h"

/* ---- Global string key-value storage implementation ---- */
#define MAX_GLOBALS 100
#define MAX_KEY_LENGTH 128
#define MAX_VALUE_LENGTH 1024

typedef struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    int used;
} GlobalValue;

static GlobalValue global_values[MAX_GLOBALS];
static int initialized = 0;

static void initialize_if_needed() {
    if (!initialized) {
        for (int i = 0; i < MAX_GLOBALS; i++) {
            global_values[i].used = 0;
        }
        initialized = 1;
    }
}

void c_set_global_value(const char* key, const char* value) {
    initialize_if_needed();

    // First, check if key already exists
    for (int i = 0; i < MAX_GLOBALS; i++) {
        if (global_values[i].used && strcmp(global_values[i].key, key) == 0) {
            strncpy(global_values[i].value, value, MAX_VALUE_LENGTH - 1);
            global_values[i].value[MAX_VALUE_LENGTH - 1] = '\0';
            return;
        }
    }

    // Find empty slot
    for (int i = 0; i < MAX_GLOBALS; i++) {
        if (!global_values[i].used) {
            strncpy(global_values[i].key, key, MAX_KEY_LENGTH - 1);
            global_values[i].key[MAX_KEY_LENGTH - 1] = '\0';

            strncpy(global_values[i].value, value, MAX_VALUE_LENGTH - 1);
            global_values[i].value[MAX_VALUE_LENGTH - 1] = '\0';

            global_values[i].used = 1;
            return;
        }
    }

    // Handle case where no slots are available
    fprintf(stderr, "Warning: Global storage full, could not add key: %s\n", key);
}

const char* c_get_global_value(const char* key) {
    initialize_if_needed();

    for (int i = 0; i < MAX_GLOBALS; i++) {
        if (global_values[i].used && strcmp(global_values[i].key, key) == 0) {
            return global_values[i].value;
        }
    }

    return NULL;
}

/* ---- Global lists implementation ---- */
#define MAX_LISTS 20
#define MAX_LIST_NAME_LENGTH 128
#define MAX_LIST_ITEMS 100

typedef struct {
    char name[MAX_LIST_NAME_LENGTH];
    char items[MAX_LIST_ITEMS][MAX_VALUE_LENGTH];
    int count;
    int used;
} GlobalList;

static GlobalList global_lists[MAX_LISTS];
static int lists_initialized = 0;

static void initialize_lists_if_needed() {
    if (!lists_initialized) {
        for (int i = 0; i < MAX_LISTS; i++) {
            global_lists[i].used = 0;
            global_lists[i].count = 0;
        }
        lists_initialized = 1;
    }
}

static GlobalList* find_or_create_list(const char* list_name) {
    initialize_lists_if_needed();

    // First, check if list already exists
    for (int i = 0; i < MAX_LISTS; i++) {
        if (global_lists[i].used && strcmp(global_lists[i].name, list_name) == 0) {
            return &global_lists[i];
        }
    }

    // Find empty slot and create new list
    for (int i = 0; i < MAX_LISTS; i++) {
        if (!global_lists[i].used) {
            strncpy(global_lists[i].name, list_name, MAX_LIST_NAME_LENGTH - 1);
            global_lists[i].name[MAX_LIST_NAME_LENGTH - 1] = '\0';
            global_lists[i].count = 0;
            global_lists[i].used = 1;
            return &global_lists[i];
        }
    }

    return NULL;
}

void c_append_to_global_list(const char* list_name, const char* value) {
    GlobalList* list = find_or_create_list(list_name);
    if (list && list->count < MAX_LIST_ITEMS) {
        strncpy(list->items[list->count], value, MAX_VALUE_LENGTH - 1);
        list->items[list->count][MAX_VALUE_LENGTH - 1] = '\0';
        list->count++;
    } else if (list) {
        fprintf(stderr, "Warning: List %s is full, could not add item\n", list_name);
    } else {
        fprintf(stderr, "Warning: Could not find or create list %s\n", list_name);
    }
}

int c_get_global_list_size(const char* list_name) {
    initialize_lists_if_needed();

    for (int i = 0; i < MAX_LISTS; i++) {
        if (global_lists[i].used && strcmp(global_lists[i].name, list_name) == 0) {
            return global_li
