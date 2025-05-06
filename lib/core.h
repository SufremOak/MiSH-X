#ifndef LIBGLOBPYCPP_CORE_H
#define LIBGLOBPYCPP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Global string key-value store functions */
void c_set_global_value(const char* key, const char* value);
const char* c_get_global_value(const char* key);

/* Global list functions */
void c_append_to_global_list(const char* list_name, const char* value);
int c_get_global_list_size(const char* list_name);
const char* c_get_global_list_item(const char* list_name, int index);

/* Clear all global data */
void c_clear_globals();

/* Counter struct and functions for object-oriented example */
typedef struct Counter {
    int value;
} Counter;

Counter* create_counter(int initial_value);
void increment_counter(Counter* counter);
int get_counter_value(const Counter* counter);
void destroy_counter(Counter* counter);

#ifdef __cplusplus
}
#endif

#endif /* LIBGLOBPYCPP_CORE_H */
