cdef extern from "core.h":
    void c_set_global_value(const char* key, const char* value)
    const char* c_get_global_value(const char* key)
    void c_append_to_global_list(const char* list_name, const char* value)
    int c_get_global_list_size(const char* list_name)
    const char* c_get_global_list_item(const char* list_name, int index)
    void c_clear_globals()

    ctypedef struct Counter:
        int value

    Counter* create_counter(int initial_value)
    void increment_counter(Counter* counter)
    int get_counter_value(Counter* counter)
    void destroy_counter(Counter* counter)

# Python interface functions
def set_global_value(key, value):
    """
    Set a global string value with the given key.

    Args:
        key (str): Key for the global value
        value (str): Value to store globally
    """
    key_bytes = key.encode('utf-8')
    value_bytes = value.encode('utf-8')
    c_set_global_value(key_bytes, value_bytes)

def get_global_value(key):
    """
    Get a global string value by key.

    Args:
        key (str): Key for the global value

    Returns:
        str: The stored value or None if key not found
    """
    key_bytes = key.encode('utf-8')
    cdef const char* result = c_get_global_value(key_bytes)
    if result == NULL:
        return None
    return result.decode('utf-8')

def append_to_global_list(list_name, value):
    """
    Append a value to a global list.

    Args:
        list_name (str): Name of the global list
        value (str): Value to append to the list
    """
    list_name_bytes = list_name.encode('utf-8')
    value_bytes = value.encode('utf-8')
    c_append_to_global_list(list_name_bytes, value_bytes)

def get_global_list(list_name):
    """
    Get all values from a global list.

    Args:
        list_name (str): Name of the global list

    Returns:
        list: List of string values
    """
    list_name_bytes = list_name.encode('utf-8')
    cdef int size = c_get_global_list_size(list_name_bytes)
    result = []

    for i in range(size):
        item = c_get_global_list_item(list_name_bytes, i)
        if item != NULL:
            result.append(item.decode('utf-8'))

    return result

def clear_globals():
    """
    Clear all global values and lists.
    """
    c_clear_globals()

# Python class wrapping the C Counter struct
cdef class GlobalCounter:
    """
    A counter object that can be incremented and tracked.
    This demonstrates how to wrap C structs in Python classes.
    """
    cdef Counter* _c_counter

    def __cinit__(self, initial_value=0):
        self._c_counter = create_counter(initial_value)
        if self._c_counter == NULL:
            raise MemoryError("Could not allocate counter")

    def __dealloc__(self):
        if self._c_counter != NULL:
            destroy_counter(self._c_counter)

    def increment(self):
        """Increment the counter by one"""
        increment_counter(self._c_counter)

    @property
    def value(self):
        """Get the current counter value"""
        return get_counter_value(self._c_counter)
